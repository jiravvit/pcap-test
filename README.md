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


# Refactoring

## 1. ip 주소 출력 관련
```
printf("%s, %s", inet_ntoa(iph->ip_src), inet_ntoa(iph->ip_dst);
```
- inet_ntoa(iph->ip_src)와 inet_ntoa(iph->ip_dst)의 출력 결과가 같다.
- C언어 함수 호출 규약 : `cdecl`
- `printf` 함수의 인자로 먼저 `inet_ntoa(iph->ip_dst)`이 들어가고, `inet_ntoa(iph->ip_src)`이 들어간다.
- `inet_ntoa` 함수의 `buf`에는 처음에 `iph->ip_dst`가 들어갔다가 `iph->ip_src`가 들어가게 된다. 그래서 `src`가 두 번 출력이 된다.

#### 해결
- [소비를 한다.](./pcap-test.cpp#L14)
    ```
    printf("%s", inet_ntoa(iph->ip_src);
    printf("%s", inet_ntoa(iph->ip_dst);
    ```
- `inet_ntop` 함수 사용해도 된다.

### (1) 
```
char* inet_ntoa(uint32_t n)
{
    char buf[16];
    ...
    return buf;
}
```
- 지역변수를 배열로 선언해서 리턴하면 스택의 포인터를 반환함

### (2)
```
char* inet_ntoa(uint32_t n)
{
    char* buf = malloc(16);
    ...
    return buf;
}
```
- `inet_ntoa`함수를 사용하고 `free`를 해줘야 하는 불편함
- 좋은 사용방법은 아님

### (3)
```
//char buf[16];

char* inet_ntoa(uint32_t n)
{
    ...
    return buf;
}
```
- 전역변수로 선언하면 굳이 메모리를 해제할 필요가 없음
- `buf`는 `inet_ntoa`함수 에서만 참조를 해야함. 다른 곳에서 보여질 필요가 없음

### (3-2)
```
//char buf[16];

char* inet_ntoa(uint32_t n)
{
    static char buf[16];
    ...
    return buf;
}
```
- 이렇게하면 전역변수로 처리한 것과 똑같이 됨
- (지역변수에 `static`을 붙이면 전역변수와 똑같음)

### (3-3)
```
//char buf[16];

char* inet_ntoa(uint32_t n)
{
    thread static char buf[16];
    ...
    return buf;
}
```
- `thread`로 변수를 선언해주면, 첫번째 쓰레드가 바라보는 buf의 위치와 두번재 쓰레드가 바라보는 buf 위치가 달라짐
- 생성자와 소멸자가 정의되어 있는 객체 내에서는 사용 금지

## 2. 데이터 영역 출력하는 방법
- 출력해야하는 데이터가 8byte보다 짧을 수도 있음
- [변수 cnt 추가](./pcap-test.cpp#L33)

## 3. 헤더 정의하기 전에 검증
- 중복되는 코드 없애면서 해결

