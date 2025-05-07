#!/bin/bash
 
# Firewall for Basic Security

. /etc/firewall.conf

# These two rules set the default policies, i.e. what to do if a packet
# doesn't match any other rule, to drop any packet coming into (INPUT)
# or routing through (FORWARD) the box.
iptables -P INPUT DROP
iptables -P FORWARD DROP
 
# These rules are added (-A) to the INPUT chain. They allow packets from
# any previously established connections and accept anything from the
# loopback interface.
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -A INPUT -s 127.0.0.1 -d 127.0.0.1 -i lo -j ACCEPT
 
# This rule added to the INPUT chain accepts any ssh connections.
for i in ${CARDS[@]}; do
	iptables -A INPUT -p tcp --dport 22 -i $i -j ACCEPT
done

# HTTP
if [ "$HTTP" = "yes" ]; then
	iptables -A INPUT -m state --state NEW -m tcp -p tcp --dport 80 -j ACCEPT
fi
