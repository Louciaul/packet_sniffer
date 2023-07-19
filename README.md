# packet_sniffer

A light packet sniffer to analyze incoming traffic

## Run the code

Use make to create the binary

```bash
make
```

Use make clean to clean the folder

```
make clean
```

Usage:

```bash
sudo ./bin/analyseur (-i <interface> | -o <fichier> | -f <filtre> | -v <1,2,3> ) 
```

-i in order to specify and listen on an interface

-o in order to analyze a file (offline)

-v verbosity (default everything is printed)

## Protocols featured

Layer 2:

- Ethernet

Layer 3:

- Ipv4
- Ipv6
- ARP/RARP

Layer 4:

- TCP
- UDP

Layer 5:

- Bootp avec DHCP
- FTP
- HTTP(S)
- POP
- IMAP
- SMTP
- TELNET

DNS not featured

## Sources:

[bootp.h](headers/bootp.h): https://samy.pl/packet/MISC/tcpdump-3.7.1/bootp.h

## Thanks

Théo B for answering multiples questions and sharing its sources for [bootp.h](headers/bootp.h)

## License

This project is under the GNU GENERAL PUBLIC LICENSE-3.0 new or revised license. Please read the [LICENSE](LICENSE) file.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.





