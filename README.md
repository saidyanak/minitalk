*This project has been created as part of the 42 curriculum by syanak.*

# Minitalk

## Aciklama

Minitalk, Unix sinyalleri (`SIGUSR1` ve `SIGUSR2`) kullanarak iki proses arasinda iletisim kuran bir projedir. Proje iki programdan olusur: **server** ve **client**. Client, server'a bir metin mesaji gonderir; server bu mesaji sinyal sinyal alip terminale yazdirir. Tum iletisim yalnizca iki sinyal uzerinden, bit bit gerceklestirilir.

### Proje Amaci

- Unix sinyallerinin (`SIGUSR1`, `SIGUSR2`) nasil calistigini anlamak
- Prosesler arasi iletisimi (IPC - Inter-Process Communication) uygulamali olarak ogrenmek
- Bitwise (bit duzeyinde) islemleri kullanarak veri iletimi gerceklestirmek

## Nasil Calisiyor?

### Genel Iletisim Akisi

```
CLIENT                                    SERVER
======                                    ======
  |                                         |
  |<-- 32 bit PID gonderimi (SIGUSR1/2) --->|  (Handshake)
  |                                         |
  |<-- 8 bit karakter gonderimi ----------->|  (Mesaj)
  |<-- SIGUSR1 (onay) --------------------->|  (Her bit icin ACK)
  |                                         |
  |<-- 8 bit NULL byte (0x00) ------------->|  (Mesaj sonu)
  |<-- SIGUSR1 (son onay) ----------------->|
  |                                         |
CIKIS                               BEKLEMEYE DEVAM
```

### 1. Sinyal-Bit Eslemesi

| Sinyal   | Anlami    |
|----------|-----------|
| SIGUSR1  | Bit `1`   |
| SIGUSR2  | Bit `0`   |

Her karakter 8 bit olarak, her bit tek bir sinyal ile iletilir. Ornegin `A` harfi (ASCII 65, binary `01000001`) icin sirasi ile `SIGUSR2, SIGUSR1, SIGUSR2, SIGUSR2, SIGUSR2, SIGUSR2, SIGUSR2, SIGUSR1` sinyalleri gonderilir.

### 2. Client PID Gonderimi (Handshake)

Client, mesaj gondermeye baslamadan once kendi PID'sini server'a bildirir. Boylece server, onay sinyallerini dogru prosese gonderebilir.

- Client PID'si 32 bit olarak MSB'den (en anlamli bit) LSB'ye dogru gonderilir
- Her bit arasinda `usleep(20)` ile 20 mikrosaniye beklenir
- Bu asamada server'dan onay beklenmez (asenkron)

```c
// client.c - PID gonderimi
void send_client_pid(int client_pid, int server_pid)
{
    int bit = 32;
    while (bit--)
    {
        if (client_pid & (1 << bit))
            kill(server_pid, SIGUSR1);   // bit 1
        else
            kill(server_pid, SIGUSR2);   // bit 0
        usleep(20);
    }
}
```

Server tarafinda `receive_pid()` fonksiyonu gelen her sinyali biriktirerek 32 bit tamamlandiginda PID'yi global degiskene yazar:

```c
// server.c - PID alma
void receive_pid(int sig)
{
    static int bit = 32;
    static int pid = 0;

    bit--;
    if (sig == SIGUSR1)
        pid = pid | (1 << bit);    // ilgili bit'i 1 yap
    if (bit == 0)
    {
        g_client_pid = pid;        // PID tamamlandi
        pid = 0;
        bit = 32;
        kill(g_client_pid, SIGUSR1); // onay gonder
    }
}
```

### 3. Mesaj Iletimi (Bit Bit)

PID gonderimi tamamlandiktan sonra client, mesajdaki her karakteri 8 bit olarak gonderir:

- Her bit gonderildikten sonra client, server'dan onay sinyali (`SIGUSR1`) bekler
- Client, `g_flag` degiskeni uzerinden busy-wait yaparak senkronizasyon saglar
- Server her bit'i aldiktan sonra `SIGUSR1` ile onay doner
- 8 bit tamamlandiginda server, byte'i karakter olarak terminale yazdirir
- Null byte (`0x00`) alindiginda mesaj tamamlanmis sayilir ve satir sonu basilir

```c
// client.c - Byte gonderimi
void send_byte(char byte, int server_pid)
{
    int i = 8;
    while (i--)
    {
        g_flag = 0;
        if (byte & (1 << i))
            kill(server_pid, SIGUSR1);
        else
            kill(server_pid, SIGUSR2);
        while (!g_flag)        // server onay verene kadar bekle
            ;
    }
}
```

### 4. Server Sinyal Yonlendirmesi

Server, gelen sinyalleri duruma gore dogru fonksiyona yonlendirir:

```c
void handle_signal(int sig)
{
    if (g_client_pid == 0)
        receive_pid(sig);     // henuz PID alinmadi -> handshake
    else
        receive_msg(sig);     // PID alindi -> mesaj al
}
```

## Talimatlar

### Derleme

```bash
make        # server ve client derlenir
make clean  # calistirilabilir dosyalar silinir
make fclean # clean ile ayni
make re     # temiz yeniden derleme
```

Derleme `cc` derleyicisi ile `-Wall -Wextra -Werror` bayraklari kullanilarak yapilir.

### Calistirma

**1. Adim:** Server'i baslatin:

```bash
./server
```

Cikti olarak server'in PID'si goruntulenir:

```
PID: 12345
```

**2. Adim:** Baska bir terminal acin ve client ile mesaj gonderin:

```bash
./client 12345 "Merhaba Dunya"
```

Server terminalinde mesaj goruntulenir:

```
Merhaba Dunya
```

### Kullanim

```
./server
./client <server_pid> <mesaj>
```

| Arguman       | Aciklama                          |
|---------------|-----------------------------------|
| `server_pid`  | Server'in terminale yazdigi PID   |
| `mesaj`       | Gonderilecek metin                |

## Teknik Detaylar

| Ozellik                  | Detay                                          |
|--------------------------|------------------------------------------------|
| Kullanilan sinyaller     | `SIGUSR1` (bit 1), `SIGUSR2` (bit 0)           |
| PID iletimi              | 32 bit, MSB-first, 20us aralik, onaysiz        |
| Mesaj iletimi            | 8 bit/karakter, MSB-first, her bit icin onay   |
| Mesaj sonu isareti       | Null byte (`0x00`)                             |
| Senkronizasyon           | Client: busy-wait, Server: sinyal tabanli      |
| Mesaj uzunlugu siniri    | Yok (sistem kaynak limitlerine kadar)          |
| Uyumluluk                | POSIX uyumlu (Linux/Unix)                      |

## Kaynaklar

- [signal(7) - Linux Man Page](https://man7.org/linux/man-pages/man7/signal.7.html) - Unix sinyalleri hakkinda detayli dokumantasyon
- [kill(2) - Linux Man Page](https://man7.org/linux/man-pages/man2/kill.2.html) - Prosese sinyal gonderme
- [sigaction(2) - Linux Man Page](https://man7.org/linux/man-pages/man2/sigaction.2.html) - Sinyal isleyici tanimlama
- [Bitwise Operators in C](https://en.wikipedia.org/wiki/Bitwise_operations_in_C) - C dilinde bitwise islemler
- [Inter-Process Communication](https://en.wikipedia.org/wiki/Inter-process_communication) - Prosesler arasi iletisim yontemleri

### Yapay Zeka Kullanimi

Bu projede yapay zeka (AI) araclari kodlama asamasinda kullanilmamistir. README dokumantasyonunun duzenlenmesinde AI destegi alinmistir.
