#!/bin/bash
#
# /usr/sbin/firewall: basic stateful firewall for IPv4 and IPv6
#
# Settings are read from /etc/firewall.conf
#

. /etc/firewall.conf

# Run the same rule for IPv4 (iptables) and IPv6 (ip6tables), so both
# kinds of network address are protected in the same way.
both() {
	iptables "$@"
	ip6tables "$@"
}

# Start from a clean state, so running this twice does not duplicate rules
both -F
both -X

# Default policies: drop everything coming in or passing through,
# allow everything going out.
both -P INPUT DROP
both -P FORWARD DROP
both -P OUTPUT ACCEPT

# Loopback: allow programs on this machine to talk to each other
# (e.g. a local Shiny app opened in your browser).
both -A INPUT -i lo -j ACCEPT

# Allow replies to connections this machine started (web pages,
# downloads, DNS lookups...), plus related traffic such as error messages.
both -A INPUT -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT

# Drop malformed / invalid packets
both -A INPUT -m conntrack --ctstate INVALID -j DROP

# ICMPv6: IPv6 needs this to find the router and other machines on the
# network. Without it, IPv6 stops working.
ip6tables -A INPUT -p ipv6-icmp -j ACCEPT

# SSH (port 22), only on the network interfaces listed in CARDS
for i in "${CARDS[@]}"; do
	both -A INPUT -i "$i" -p tcp --dport 22 -m conntrack --ctstate NEW -j ACCEPT
done

# Extra ports listed in TCP_PORTS (see /etc/firewall.conf for examples)
for p in "${TCP_PORTS[@]}"; do
	both -A INPUT -p tcp --dport "$p" -m conntrack --ctstate NEW -j ACCEPT
done
