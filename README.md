# pcap-test
- BoB 10th 3차 공통 교육 과제 #3
- 송수신되는 packet을 capture하여 중요 정보를 출력하는 C/C++ 기반 프로그램을 작성하라.

# 실행
## libnet 설치
```
sudo apt update
sudo apt install -y libnet-dev  
```
## 실행 방법
```
syntax: pcap-test <interface>
sample: pcap-test wlan0
```
## 출력 결과(예시)
```
===================================================

[*] Header
[+] Ethernet 
    src mac >> 90:9f:33:d9:a0:e3
    dst mac >> b4:2e:99:ea:97:de
[+] IP 
    src ip >> 175.213.35.39
    dst ip >> 10.1.1.3
[+] TCP 
    src port >> 80
    dst port >> 59208
[*] Payload
[+] DATA (8 bytes) 
    >> 48 54 54 50 2F 31 2E 31 

```

# 기능
- 송수신되는 packet을 capture하여 아래 중요 정보 출력
## 상세
1. Ethernet Header의 src mac / dst mac
2. IP Header의 src ip / dst ip
3. TCP Header의 src port / dst port
4. Payload(Data)의 hexadecimal value(최대 8바이트까지만)

# 비고
- TCP packet이 잡히는 경우 "ETH + IP + TCP + DATA" 로 구성이 됨. 이 경우(TCP packet이 잡혔다고 판단되는 경우만)에만 1~4의 정보를 출력하도록 함(Data의 크기가 0이어도 출력).
- libnet 구조체 참고
  - http://packetfactory.openwall.net/projects/libnet > Latest Stable Version: 1.1.2.1 다운로드(libnet.tar.gz) > include/libnet/libnet-headers.h
- [Dummy interface를 이용하여 디버깅을 쉽게할 수 있는 방법](https://gilgil.gitlab.io/2020/07/23/1.html)
- [pcap-test 코드](https://gitlab.com/gilgil/pcap-test/-/blob/master/pcap-test.c)를 skeleton code으로 하여 과제를 수행
