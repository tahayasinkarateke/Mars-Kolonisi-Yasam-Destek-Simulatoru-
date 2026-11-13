# Mars-Kolonisi-Yasam-Destek-Simulatoru-
 C Dili İle Karakter Tabanli Mars Kolonisi Yasam Destek Simulatoru 
Mars Kolonisi Yaşam Destek Simülatörü

C diliyle yazılmış, metin tabanlı ve tek karakterli komutlarla kontrol edilen bir hayatta kalma oyunu. Oyuncu, Mars'taki küçük bir araştırma istasyonunda tek başına kalan bir mühendisi yönetir ve kurtarma ekibi gelene kadar 15 gün hayatta kalmaya çalışır.

Algoritma ve Programlama dersi ödevi · Taha Yasin Karateke · 1260505001

İçindekiler
Özellikler
Derleme ve Çalıştırma
Komutlar
Zorluk Seviyeleri
Oyun Kuralları
Örnek Çıktı
Kullanılan C Yapıları
Proje Yapısı
Özellikler
Sağlık, enerji, oksijen, su, yedek parça, modül hasarı, gün ve tur değerlerini takip eden durum sistemi
Üç zorluk seviyesi (Kolay, Normal, Zor); seviye başlangıç değerlerini ve olay eşiklerini değiştirir
Büyük/küçük harf duyarsız tek karakterli komutlar
Rastgele olaylarda zar ve eşik değerinin ekrana yazdırılması (örn. [Kesif Zari]: 47 / Esik: 60 -> BASARILI)
Kum fırtınası simülasyonu ve gün bazlı değişen şifreyle hava kilidi mini oyunu
Oyun nasıl biterse bitsin sonuç raporu ve puan
Derleme ve Çalıştırma

Herhangi bir C derleyicisi yeterlidir (örnek: GCC).

bash
gcc -Wall -Wextra -o mars main.c
./mars

Windows'ta çalıştırmak için ./mars yerine mars.exe kullanılır.

Komutlar

Komutlar büyük veya küçük harfle girilebilir (g ve G aynı işlemi yapar). Tanımsız bir komut uyarı verir ve tur harcamaz.

Komut	İşlev	Ayrıntılar
G	Keşif	En az 15 enerji gerekir. Enerji −15, oksijen −10. Yaralanma zarı zar < 10 || modulHasari > 70 ise sağlık −15. Bulma eşiği enerji > 40 && oksijen > 30 ise yüksek, değilse düşüktür. Sonuç: 4 su, 2 yedek parça veya eli boş dönüş
O	Oksijen üret	En az 5 enerji ve 1 su gerekir. Her 2 su 3 oksijene dönüşür. Tek miktar girilirse % 2 ile 1 birim artık su depoda kalır. Enerji −5. Mevcut sudan fazlası girilemez
T	Tamir	En az 10 enerji ve 1 yedek parça gerekir. Hasar kademesine göre parça harcanır (aşağıdaki tabloya bakın). Enerji −10
D	Durum raporu	Tüm değerleri ve kritik uyarıları gösterir. Tur harcamaz
U	Uyu / dinlen	Enerji +25, oksijen −5. Oksijen 20'nin altındaysa sağlık −10, değilse +15
K	Kum fırtınası	Zorluğa göre 3/5/7 turluk fırtına; can kaybı, oksijen sızıntısı veya modül hasarı olabilir
H	Hava kilidi	En az 15 enerji gerekir. 3 denemede şifreyi bulursan kilit mühürlenir
Q	Çıkış	Oyunu sonlandırır ve sonuç raporunu yazdırır

Enerjisi yetmeyen komut çalıştırılmaz, iptal edilir ve tur harcanmaz.

Tamir kademeleri
Modül hasarı	Seviye	Harcanan parça	Parça başına onarım
0 - 30	Hafif	1	15
31 - 70	Orta	2 (yoksa 1)	15
71 - 100	Ağır	3 (yoksa eldeki kadar)	12
Kum fırtınası (K)

Her turda rand() % 100 ile zar atılır ve zar sırasıyla şu aralıklara göre değerlendirilir:

Zar	Sonuç	Hasar
< sağlık eşiği	Enkaz: can kaybı	10
< sızıntı eşiği	Basınç düşüşü: oksijen sızıntısı	15
< modül eşiği	Panel hasarı: modül hasarı artışı	12
Diğer	Zararsız atlatma	-
Çift numaralı turlarda rüzgâr şiddetlenir ve hasar 2 katına çıkar (t % 2 == 0).
Hava kilidi mühürlüyse (kilitMuhurlu == 1) oksijen sızıntısı o tur için continue ile atlanır.
Sağlık veya oksijen 0'a düşerse fırtına break ile hemen sona erer.
Fırtına sonunda özet yazdırılır: hasar alınan tur sayısı, toplam can kaybı, oksijen kaybı ve modül hasarı.
Hava kilidi şifresi (H)
Şifre sabit değildir; güne göre hesaplanır: sifre = 'A' + (gun % 26). Oyuncuya "A harfinden bugünün gün sayısı kadar ilerideki harf" ipucu verilir (1. gün B, 2. gün C, ...).
Küçük harf girişi de kabul edilir.
3 deneme hakkı vardır. Her yanlış denemede enerji −5 olur ve "ILERI" veya "GERI" ipucu verilir.
Doğru şifre kilidi mühürler (kilitMuhurlu = 1) ve kum fırtınalarındaki oksijen sızıntısını engeller.
Üç deneme de başarısız olursa modül hasarı +20 artar.
Zorluk Seviyeleri

Başlangıçta K, N veya Z girilir. Geçersiz bir harf girilirse seçim tekrar istenir.

	Kolay (K)	Normal (N)	Zor (Z)
Sağlık / Enerji / Oksijen	100 / 100 / 100	100 / 100 / 100	80 / 80 / 80
Su	15	10	5
Yedek parça	5	3	1
Modül hasarı	%0	%10	%25
Fırtına süresi	3 tur	5 tur	7 tur
Keşif bulma eşiği (yüksek / düşük)	70 / 40	60 / 30	50 / 20
Fırtına eşikleri (can / sızıntı / modül)	15 / 35 / 60	25 / 50 / 75	35 / 60 / 85
Oyun Kuralları
Zaman akışı
Geçerli her komut turu 1 artırır. D komutu ve tanımsız komutlar tur harcamaz.
Her 3 turda bir (turSayaci % 3 == 0) yeni gün başlar: oksijen −8 olur. Modül hasarı %70'in üzerindeyse ek olarak sağlık −5 olur.
Gün sayısı en fazla 15'e çıkar.
Değer sınırları

Sağlık, enerji, oksijen ve modül hasarı her komuttan sonra 0-100 aralığında tutulur. Fazla üretilen oksijen veya fazla uyku 100'ün üzerine çıkmaz.

Oyun sonu

Koşullar her turun sonunda kontrol edilir:

Durum	Sonuç
Sağlık ≤ 0	Kaybettin: hayatını kaybettin
Oksijen ≤ 0	Kaybettin: oksijen tükendi
15. gün tamamlandı (45. tur)	Kazandın: kurtarma ekibi geldi
Q komutu	Oyundan çıkış

Her durumda sonuç raporu yazdırılır: tamamlanan gün, son gün, toplam tur, son sağlık/oksijen/enerji/hasar değerleri, kalan su ve yedek parça, hava kilidi durumu ve puan.

Puan
Puan = (gun × 100) + saglik + oksijen − modulHasari

Hayatta kalınan her gün 100 puan kazandırır; sağlam ve oksijeni dolu bitirmek ek puan getirir, istasyonu bakımsız bırakmak puanı düşürür.

Örnek Çıktı

Aşağıda Normal seviyede keşif (G) ve oksijen üretimi (O) komutlarının örnek çıktısı görülür (zarlar her çalıştırmada değişir):

text
Zorluk Seviyesi Seciniz (K: Kolay, N: Normal, Z: Zor): n

[ZORLUK]: NORMAL mod secildi.

----------------------------------------------------
GUN: 1 | TUR: 0 | Saglik: 100 | Enerji: 100 | Oksijen: 100 | Su: 10 | Parca: 3 | Hasar: %10
Komut Giriniz: g

[Yaralanma Zari]: 77 / Esik: 10 -> GUVENLI
[Kesif Zari]: 99 / Esik: 60 -> ELI BOS DONDUNUZ (Hicbir sey bulunamadi).

----------------------------------------------------
GUN: 1 | TUR: 1 | Saglik: 100 | Enerji: 85 | Oksijen: 90 | Su: 10 | Parca: 3 | Hasar: %10
Komut Giriniz: o

Kac birim su kullanilsin? (Mevcut Su: 10): 5

[URETIM]: 4 birim su kullanildi (1 birim artik su depoda kaldi).
[URETIM]: +6 birim Oksijen uretildi.

Geçersiz giriş örneği:

text
Zorluk Seviyesi Seciniz (K: Kolay, N: Normal, Z: Zor): x
[HATA]: Gecersiz zorluk seviyesi! Lutfen K, N veya Z giriniz.
Kullanılan C Yapıları
Yapı	Nerede kullanıldı
switch-case ve fall-through	Komut yönetimi (case 'g': case 'G':) ve zorluk seçimi; default ile geçersiz giriş
Aritmetik ve mod (%) operatörü	Oksijen dönüşümü, artık su, gün hesabı (turSayaci % 3), çift tur kontrolü (t % 2), şifre üretimi (gun % 26)
Mantıksal operatörler (&&, ||)	Keşif eşiği ve yaralanma koşulu, durum raporunda "tüm sistemler normal" kontrolü, şifre döngüsü koşulu
if - else if - else	Tamir kademeleri, keşif sonuçları, iç içe dinlenme koşulu, oyun sonu kontrolü
for döngüsü, break, continue	Kum fırtınası simülasyonu
do-while döngüsü	Zorluk seçimi, ana komut döngüsü, 3 denemeli hava kilidi şifresi
Fonksiyon ve işaretçi	degerleriSinirla() değerleri işaretçiyle alıp 0-100 aralığına çeker; suMiktariOku() satırın tamamını kontrol ederek sayı okur
rand() ve srand(time(NULL))	Tüm rastgele olaylar


