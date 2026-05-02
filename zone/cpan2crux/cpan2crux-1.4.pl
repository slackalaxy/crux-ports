#!/usr/bin/perl
use strict;
use warnings;
use CPAN;
use CPAN::Meta;
use CPAN::DistnameInfo;
use CPAN::HandleConfig;
use Getopt::Long;
use Pod::Usage;
use Pod::Man;
use Cwd;
use Digest::MD5;
use Data::Dumper;
use Term::ANSIColor;
$Data::Dumper::Indent   = 3;
$Data::Dumper::Sortkeys = 1;
$Data::Dumper::Terse    = 1;

=head1 NAME

cpan2crux - Generate CRUX ports for CPAN Perl modules

=head1 SYNOPSIS

cpan2crux [OPTIONS] <module-name> ...

=head1 DESCRIPTION

This script will generate CRUX ports for Perl modules available in CPAN.

=head1 OPTIONS

=over 4

=item B<< {-v|--verbose} [<INT>] >>

 Print verbose information.
 Specify 2 or use flag twice for extra debugging.

=item B<< {-r|--recursive} [<INT>] >>

 Generate dependencies recursively.
 Specify 2 or use flag twice to build optional dependencies too.

=item B<< {-m|--maintainer} [<STRING>] >>

 Show or set the "Maintainer" to use when writing new ports.
 This is saved in your CPAN's configuration file.

=back

=head1 SEE ALSO

L<cpan(1pm)> - The cpan program.

L<CPAN::Meta::Spec(3pm)> - The specification for CPAN distribution metadata.

=head1 AUTHOR

Alan Mizrahi, alan at mizrahi dot com dot ve

=cut

my $verbose    = 0;   # 0 = no verbose, 1 = basic, 2 = detailed
my $recursive  = 0;   # 0 = no, 1 = required, 2 = required and optional
my $maintainer;

$SIG{__WARN__} = sub { print colored(['magenta'], @_); };
#$SIG{__DIE__} = sub { print colored(['red'], @_) unless $^S; };

GetOptions('help' => sub { showUsage(0) }, 'debug|verbose:+' => \$verbose, 'recursive:+' => \$recursive, 'maintainer:s' => \$maintainer) or showUsage(1);

if (defined $maintainer) {
	if ($maintainer eq '') {
		# show maintainer in CPAN's configuration
		CPAN::HandleConfig->load;
		CPAN::HandleConfig->prettyprint('crux_maintainer');
	} else {
		# save maintainer in CPAN's configuration
		$CPAN::HandleConfig::keys{'crux_maintainer'} = undef;
		CPAN::HandleConfig->load;
		CPAN::HandleConfig->edit('crux_maintainer', $maintainer);
		CPAN::HandleConfig->commit;
	}
}
exit 0 unless $#ARGV > -1;

print "Building list of Perl modules included in perl package\n" if $verbose >= 1;
my $perlDistroModules = getPerlDistroModules();

my $dists; # hashref of dist-name => { dist => CPAN::Distribution, info -> CPAN::DistnameInfo, status => 'pending'|'done'|'failed' }

# figure out distributions from modules in @ARGV
$dists = getModDeps( { map { $_ => 1 } @ARGV } );

# initially assign them the 'pending' status
foreach my $name (keys %{$dists}) {
	$dists->{$name}{status} = 'pending';
}

# repeat until there are no more with 'pending' status
# two loops are needed because the hash changes in the inner loop
while ( scalar( grep { $dists->{$_}{status} eq 'pending' } keys %{$dists}) > 0 ) {
	DIST:
	foreach my $name (sort grep { $dists->{$_}{status} eq 'pending' } keys %{$dists}) {
		my $ns = dist2ns($name);
		my $dist = $dists->{$name}{dist};

		print "=======> Working on $ns\n";
		print "Distribution:\n", Dumper($dist), "\n---\n" if $verbose >= 2;
		print "Downloading and unpacking ", $dist->id, "\n" if $verbose >= 1;

		# CPAN::Distribution->get() changes working directory
		my $oldwd = getcwd;
		$dist->get();
		chdir($oldwd);
		
		print "Extracted to: ", $dist->dir, "\n" if $verbose >= 1;
		
		# The specifications about CPAN metadata are in: CPAN::Meta::Spec
		my $meta;
		foreach my $metafile ( map { $dist->dir.'/'.$_ } ('META.json', 'META.yml') ) {
			print "Looking for $metafile\n" if $verbose;
			if (-f $metafile) {
				$meta = CPAN::Meta->load_file($metafile);
				last;
			}
		}
		
		if (! defined $meta) {
			warn "WARNING: could not get metadata for ", $dist->base_id," in ", $dist->dir, "\n";
			my $author = $dist->author;
			warn "Please ask ", $author->fullname," <".$author->email."> to write proper metadata for $ns\n" if defined $author;
			warn "Will generate crappy port without dependency information\n";
		}
		
		print "Metadata:\n", Dumper($meta), "\n---\n" if $verbose >= 2;
		
		my %port = (
			description  => '',
			url          => '',
			dependencies => {},
			optional     => {},
			name         => dist2port($dists->{$name}{info}{dist}),
			version      => $dists->{$name}{info}{version},
			release      => '1',
			source       => '(http://search.cpan.org/CPAN/authors/id/'. $dist->id=~s{$dists->{$name}{info}{version}}{\${version}}r .')',
			build        => {
				dir => "$name-\$version",
				cmd => '# Write your compilation commands here',
				}
		);

		if (-d $port{name}) {
			print "ERROR: directory $port{name} already exists\n";
			$dists->{$name}{status} = 'failed';
			next DIST;
		}
		
		# get best possible description
		if ( defined $meta->{abstract} && ! ($meta->{abstract} eq '') && ! ($meta->{abstract} =~ /^unknown$/i) ) {
			$port{description} = ucfirst $meta->{abstract};
		} else {
			$port{description} = $ns;
		}
		
		# find the distribution's URL
		if ( defined $meta->{resources}{homepage} ) {
			$port{url} = $meta->{resources}{homepage};
		} elsif ( defined $dists->{$name}{info}{cpanid} ) {
			# TODO: is it necessary to encode the cpanid or name?
			$port{url} = sprintf('http://search.cpan.org/~%s/%s/', lc $dists->{$name}{info}{cpanid}, $name);
		} elsif ( defined $meta->{resources}{repository}{web} ) {
			$port{url} = $meta->{resources}{repository}{web};
		} elsif ( defined $meta->{resources}{repository}{url} ) {
			$port{url} = $meta->{resources}{repository}{url};
		}
		
		if ( $meta->{dynamic_config} ) {
			warn "WARNING: $ns uses dynamic config, please verify dependencies in $port{name}/Pkgfile\n";
		}
		
		# build hash based on runtime dependencies
		for my $phase ('build', 'configure', 'runtime') {
			print "Adding dependencies for $phase phase\n" if $verbose >= 3;
			%{$port{dependencies}} = ( %{$port{dependencies}}, %{getModDeps($meta->{prereqs}{$phase}{requires})} );
		}
		
		# get the optional ports based on $meta->{prereqs}{runtime}{recommends} 
		# and $meta->{prereqs}{runtime}{suggests}
		for my $phase ('build', 'configure', 'runtime') {
			print "Adding optional dependencies for $phase phase\n" if $verbose >= 3;
			for my $what ('recommends', 'suggests') {
				%{$port{optional}} = ( %{ $port{optional} }, %{ getModDeps($meta->{prereqs}{$phase}{$what}) } );
			}
		}
		
		# now merge each additional optional_features dependencies
		# from $meta->{optional_features}{*}{prereqs}{runtime}
		if ( defined $meta->{optional_features} ) {
			foreach my $feature (keys %{$meta->{optional_features}}) {
				for my $phase ('build', 'configure', 'runtime') {
					print "Adding dependencies for optional feature: $feature\n" if $verbose >= 3;
					%{ $port{optional} } = ( %{ $port{optional} }, %{ getModDeps($meta->{optional_features}{$feature}{prereqs}{$phase}) } ) if defined $meta->{optional_features}{$feature}{prereqs}{$phase};
				}
			}
		}
		
		# remove all optional dependencies that are already required dependencies:
		%{ $port{optional} } = map { $_ => 1 } grep { !defined($port{dependencies}{$_}) } keys %{ $port{optional} };
		
		# add dependencies in recursive mode
		if ($recursive >= 1) {
			# figure out distributions from modules in @ARGV
			my $depdists = getModDeps($port{dependencies});

			# remove the ones that are already in $dists?
			# TODO: investigate if this can be done automatically as part of a hash merge
			foreach my $name (keys %{$dists}) {
				delete $depdists->{$name};
			}
			print "Will write ports for new dependencies: ", join(', ', keys %{$depdists}), "\n" if $verbose >= 1 && scalar(keys %{$depdists}) > 0;
			
			# initially assign them the 'pending' status
			foreach my $name (keys %{$depdists}) {
				$depdists->{$name}{status} = 'pending';
			}

			# merge the new dependencies into $dists
			%{ $dists } = ( %{$dists}, %{$depdists} );
		}
		
		# add optional dependencies in recursive mode
		if ($recursive >= 2) {
			# figure out distributions from modules in @ARGV
			my $depdists = getModDeps($port{optional});

			# remove the ones that are already in $dists?
			# TODO: investigate if this can be done automatically as part of a hash merge
			foreach my $name (keys %{$dists}) {
				delete $depdists->{$name};
			}
			
			# initially assign them the 'pending' status
			foreach my $name (keys %{$depdists}) {
				$depdists->{$name}{status} = 'pending';
			}

			print "Will write ports for new optional dependencies: ", join(', ', keys %{$depdists}), "\n" if $verbose >= 1 && scalar(keys %{$depdists}) > 0;

			# merge the new dependencies into $dists
			%{ $dists } = ( %{$dists}, %{$depdists} );
		}
		
		# convert dependencies hashes to strings of CRUX port names
		$port{dependencies} = join(' ', sort map { dist2port($_) } keys %{ $port{dependencies} } );
		$port{optional} = join(' ', sort map { dist2port($_) } keys %{ $port{optional} } );

		# if there is no dependencies, make perl the sole dependency
		$port{dependencies} = 'perl' if $port{dependencies} eq '';

		if (-f $dist->dir . '/Makefile.PL') {
			$port{build}{cmd} = "# PERL_MM_USE_DEFAULT=1 \\\n\tperl Makefile.PL\n\tmake OPTIMIZE=\"\$CFLAGS\" DESTDIR=\"\$PKG\" install";
		} elsif (-f $dist->dir . '/Build.PL') {
			$port{build}{cmd} = "# PERL_MM_USE_DEFAULT=1 \\\n\tperl Build.PL --destdir=\"\$PKG\"\n\tperl Build\n\tperl Build install";
		} else {
			warn "WARNING: Could not find Makefile.PL or Build.PL for $ns\n";
		}
		
		my $pkgfile = <<PART1;
# Description:	$port{description}
# URL:		$port{url}
# Maintainer:	$CPAN::Config->{crux_maintainer}
# Depends on:	$port{dependencies}
PART1
		# add optional dependencies, if any exist
		$pkgfile .= "# Optional:	$port{optional}\n" unless $port{optional} eq '';
		$pkgfile .= <<PART2;

name=$port{name}
version=$port{version}
release=$port{release}
source=$port{source}

build() {
	cd $port{build}{dir}

	$port{build}{cmd}

	find "\$PKG" -depth \\
		-type f \\( -name .packlist -o -name \\*.bs -o -name perllocal.pod \\) -delete , \\
		-type f -name autosplit.ix \\( -exec grep -q '^[[:space:]]*package' {} \\; -o -delete \\) , \\
		-type d -empty -exec rmdir {} +
}
PART2

		print "Port:\n", Dumper(\%port), "\n---\n" if $verbose >= 2;

		print "Writing $port{name}/Pkgfile\n" if $verbose >= 1;
		mkdir($port{name}) or do {
			print "Error creating directory $port{name}: $!\n";
			next DIST;
		};
		open PKGFILE, ">$port{name}/Pkgfile" or do {
			print "Error creating $port{name}/Pkgfile: $!\n";
			next DIST;
		};
		print PKGFILE $pkgfile;
		close PKGFILE;
		
		# Calculate md5 of source file
		print "Writing $port{name}/.md5sum\n" if $verbose >= 1;
		my $md5 = Digest::MD5->new;
		my $fh;
		if (! open($fh, "<$dist->{localfile}")) {
			print "Error reading from $dist->{localfile}: $!\n";
			$dists->{$name}{status} = 'failed';
			next DIST;
		}
		$md5->addfile($fh);
		close($fh);

		# Create .md5sum
		open (MD5SUM, ">$port{name}/.md5sum") or do {
			print "Error creating $port{name}/.md5sum: $!\n";
			$dists->{$name}{status} = 'failed';
			next DIST;
		};
		my ($basename) = $dist->{localfile} =~ /\/([^\/]+)$/;
		print MD5SUM $md5->hexdigest, "  $basename\n"; 
		close(MD5SUM);
		print "=======> Created $port{name}\n";
		$dists->{$name}{status} = 'done';
	}
}

print "\nResults:\n";
print "Wrote ", scalar (grep { $dists->{$_}{status} eq 'done' } keys %{$dists}), " ports.\n";
print "Failures: ", join(', ', sort map { dist2ns($_) } grep { $dists->{$_}{status} eq 'failed' } keys %{$dists} ), "\n";
exit 0;

# convert distribution name to port name
# ie: HTML-Tree -> p5-html-tree
sub dist2port {
	my ($name) = @_;
	$name =~ s{\W+}{-}g;
	return 'p5-' . lc $name;
}

# convert distribution name to perl namespace
# ie: HTML-Tree -> HTML::Tree
sub dist2ns {
	my ($name) = @_;
	$name =~ s{-}{::}g;
	return $name;
}

# return hash of perl modules that were installed as part of the distribution
# TODO: read database directly or write module to do this?
sub getPerlDistroModules {
	# inc contains absolute include paths from @INC without first / but with trailing /
	my @inc = map { substr($_, 1) . '/' } grep { /^\// } @INC;
	my %ret;
	
	if (open PKGINFO, 'pkginfo -l perl |') {
		DIST:
		while (<PKGINFO>) {
			my $file = $_;
			chomp($file);
			next unless $file =~ /\.pm$/;
			
			INCDIR:
			# TODO: change this to File::Spec rel2abs
			foreach my $path (@inc) {
				if (substr($file, 0, length($path)) eq $path) {
					# found $file in $path, now convert to module name
					$file = substr($file, length($path)); # remove matched path
					($file) = $file =~ /^(.+)\.pm$/; # remove extension
					$file =~ s{/}{::}g; # transform / to ::
					$ret{$file} = 1;
					next DIST;
				}
			}
			warn "WARNING: Could not find include path for $file\n";
			
		}
		close PKGINFO;
	}
	return \%ret;
}

# return distribution object from module name
sub getDistribution {
	my $module = CPAN::Shell->expand('Distribution', $_[0]);
	$module = CPAN::Shell->expand('Module', $_[0]) unless defined $module;
	$module = CPAN::Shell->expand('Bundle', $_[0]) unless defined $module;
	return undef unless defined $module;
	return $module->distribution();
}

# takes hash of Perl module dependencies and returns a hash of module distribution dependencies that are not included in the Perl port
# excluding modules that are part of the Perl distribution
# For example:
# argument: { 'Scalar::Util' => '0', 'Exporter' => '0', 'HTML::Tagset' => '3.02', 'integer' => '0', 'HTML::Parser' => '3.46', 'HTML::Entities' => '0', 'perl' => '5.008', 'Carp' => '0' }
# returns:  { 'HTML::Parser' => { dist => CPAN::Distribution, info -> CPAN::DistnameInfo }, 'HTML::Tagset' => { dist => CPAN::Distribution, info -> CPAN::DistnameInfo }}
sub getModDeps {
	my ($deps) = @_;
	my $ret = {};
	
	warn " *** getModDeps started with:\n", Dumper($deps), "---\n" if $verbose >= 3;
	
	# build hash based on runtime dependencies
	foreach my $mod ( keys %{ $deps } ) {
		next if $mod eq 'perl' or defined $perlDistroModules->{$mod};
		$mod = dist2ns($mod);
		warn " *** running getDistribution on $mod\n" if $verbose >= 3;
		my $dist = getDistribution($mod);
		if (!defined $dist) {
			warn "WARNING: could not find $mod (no match in cpan), manual check needed.\n";
			next;
		}
		my $info = CPAN::DistnameInfo->new($dist->id);
		if (!defined $info) {
			warn "WARNING: could not parse information of $dist->id, manual check needed.\n";
			next;
		}
		$ret->{$info->dist} = {dist => $dist, info => $info};
		warn " *** CPAN::DistnameInfo on ",$dist->id," returned:\n", Dumper($info),"---\n" if $verbose >= 3;
	}
	warn " *** getModDeps returning:\n", Dumper($ret), "---\n" if $verbose >= 3;

	return $ret;
}

sub showUsage {
	pod2usage(-verbose => 2, -noperldoc => 1, -exit => 'NOEXIT');
	exit(shift);
}
