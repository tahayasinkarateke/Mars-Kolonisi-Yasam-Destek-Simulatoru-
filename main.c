// Mars Kolonisi Yasam Destek Simulatoru
// Algoritma ve Programlama - Odev
// Taha Yasin Karateke - 1260505001
//
// Bu programda Mars'taki bir istasyonda tek basina kalan bir muhendisi yonetiyoruz.
// Kurtarma ekibi 15 gun sonra geliyor, o zamana kadar hayatta kalmamiz gerekiyor.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

/* Degerleri 0-100 aralıgında sınırlandıran yardımcı fonksiyon */
void degerleriSinirla(int *saglik, int *enerji, int *oksijen, int *modulHasari) {
    if (*saglik > 100) *saglik = 100;
    if (*saglik < 0)   *saglik = 0;
    
    if (*enerji > 100) *enerji = 100;
    if (*enerji < 0)   *enerji = 0;
    
    if (*oksijen > 100) *oksijen = 100;
    if (*oksijen < 0)   *oksijen = 0;
    
    if (*modulHasari > 100) *modulHasari = 100;
    if (*modulHasari < 0)   *modulHasari = 0;
}

/* Tamsayı girişini satırın tamamını kontrol ederek okur. */
static int suMiktariOku(int *miktar) {
    char satir[128], *son;
    if (scanf(" %127[^\n]", satir) != 1) return 0;
    int karakter = getchar();
    if (karakter != '\n' && karakter != EOF) {
        while ((karakter = getchar()) != '\n' && karakter != EOF) {}
        return 0;
    }
    errno = 0;
    long deger = strtol(satir, &son, 10);
    if (son == satir || errno == ERANGE || deger < INT_MIN || deger > INT_MAX)
        return 0;
    while (isspace((unsigned char)*son)) son++;
    if (*son != '\0') return 0;
    *miktar = (int)deger;
    return 1;
}

int main(void) {
    /* Durum degiskenleri */
    int saglik = 100;
    int enerji = 100;
    int oksijen = 100;
    int su = 10;
    int yedekParca = 3;
    int modulHasari = 0;
    int kilitMuhurlu = 0;
    int gun = 1;
    int turSayaci = 0;
    
    char zorluk = ' ';
    int firtinaSuresi = 5;
    int kesifEsikYuksek = 60, kesifEsikDusuk = 30;
    int firtinaSaglikEsigi = 25, firtinaSizintiEsigi = 50, firtinaHasarEsigi = 75;
    int oyunDevam = 1;
    char komut = ' ';
    int cikisKomutu = 0;

    /* Rastgele sayı üreteci baslatma */
    srand((unsigned int)time(NULL));

    printf("====================================================\n");
    printf("   MARS KOLONISI YASAM DESTEK SIMULATORU (C v1.0)   \n");
    printf("====================================================\n\n");

    /* Zorluk Seviyesi Secimi (SWITCH-CASE ile) */
    int gecerliZorluk = 0;
    do {
        printf("Zorluk Seviyesi Seciniz (K: Kolay, N: Normal, Z: Zor): ");
        if (scanf(" %c", &zorluk) != 1) return 0;
        zorluk = (char)toupper((unsigned char)zorluk);

        switch (zorluk) {
            case 'K':
                saglik = 100; enerji = 100; oksijen = 100;
                su = 15; yedekParca = 5; modulHasari = 0;
                firtinaSuresi = 3;
                kesifEsikYuksek = 70; kesifEsikDusuk = 40;
                firtinaSaglikEsigi = 15; firtinaSizintiEsigi = 35; firtinaHasarEsigi = 60;
                gecerliZorluk = 1;
                printf("\n[ZORLUK]: KOLAY mod secildi. Baslangic kaynaklari artirildi.\n");
                break;
            case 'N':
                saglik = 100; enerji = 100; oksijen = 100;
                su = 10; yedekParca = 3; modulHasari = 10;
                firtinaSuresi = 5;
                gecerliZorluk = 1;
                printf("\n[ZORLUK]: NORMAL mod secildi.\n");
                break;
            case 'Z':
                saglik = 80; enerji = 80; oksijen = 80;
                su = 5; yedekParca = 1; modulHasari = 25;
                firtinaSuresi = 7;
                kesifEsikYuksek = 50; kesifEsikDusuk = 20;
                firtinaSaglikEsigi = 35; firtinaSizintiEsigi = 60; firtinaHasarEsigi = 85;
                gecerliZorluk = 1;
                printf("\n[ZORLUK]: ZOR mod secildi. Baslangic kaynaklari kısıtli!\n");
                break;
            default:
                printf("[HATA]: Gecersiz zorluk seviyesi! Lutfen K, N veya Z giriniz.\n");
                break;
        }
    } while (!gecerliZorluk);

    printf("\nSimulasyon Basliyor... 15 Gun Hayatta Kalin!\n");

    /* Ana Oyun Dongusu (DO-WHILE zorunlulugu) */
    do {
        printf("\n----------------------------------------------------\n");
        printf("GUN: %d | TUR: %d | Saglik: %d | Enerji: %d | Oksijen: %d | Su: %d | Parca: %d | Hasar: %%%d\n",
               gun, turSayaci, saglik, enerji, oksijen, su, yedekParca, modulHasari);
        printf("Komutlar: [G] Kesif | [O] Oksijen Uret | [T] Tamir | [D] Durum | [U] Uyut/Dinlen\n");
        printf("          [K] Kum Firtinasi | [H] Hava Kilidi | [Q] Cikis\n");
        printf("Komut Giriniz: ");
        if (scanf(" %c", &komut) != 1) break;

        int gecerliTurKomutu = 0; // Tur sayacını artırıp artırmayacagını takip eder

        /* Komut Yönetimi (SWITCH-CASE ve Fall-Through) */
        switch (komut) {
            case 'g':
            case 'G': {
                /* Kesif Görevi */
                if (enerji < 15) {
                    printf("\n[YETERSIZ KAYNAK]: Kesif icin en az 15 Enerji gerekli!\n");
                    break;
                }
                enerji -= 15;
                oksijen -= 10;
                gecerliTurKomutu = 1;

                /* Yaralanma Kontrolü (IF ve Mantıksal Operatörler) */
                int zarYaralanma = rand() % 100;
                int esikYaralanma = 10;
                printf("\n[Yaralanma Zari]: %d / Esik: %d", zarYaralanma, esikYaralanma);
                if (zarYaralanma < esikYaralanma || modulHasari > 70) {
                    saglik -= 15;
                    printf(" -> BASARISIZ (Yaralandiniz! -15 Saglik)\n");
                } else {
                    printf(" -> GUVENLI\n");
                }

                /* Su / Parca Bulma Esiği Belirleme */
                int esikBulma;
                if (enerji > 40 && oksijen > 30) {
                    esikBulma = kesifEsikYuksek; // Yüksek ihtimal
                } else {
                    esikBulma = kesifEsikDusuk; // Düsük ihtimal
                }

                int zarKesif = rand() % 100;
                printf("[Kesif Zari]: %d / Esik: %d", zarKesif, esikBulma);
                
                if (zarKesif < esikBulma) {
                    int secimZari = rand() % 100;
                    printf(" -> BASARILI\n[Kaynak Zari]: %d / Esik: 50", secimZari);
                    if (secimZari < 50) {
                        su += 4;
                        printf(" -> 4 birim SU bulundu!\n");
                    } else {
                        yedekParca += 2;
                        printf(" -> 2 adet YEDEK PARCA bulundu!\n");
                    }
                } else {
                    printf(" -> ELI BOS DONDUNUZ (Hicbir sey bulunamadi).\n");
                }
                break;
            }

            case 'o':
            case 'O': {
                /* Oksijen Üretimi */
                if (enerji < 5) {
                    printf("\n[YETERSIZ KAYNAK]: Oksijen uretimi icin en az 5 Enerji gerekli!\n");
                    break;
                }
                if (su <= 0) {
                    printf("\n[YETERSIZ KAYNAK]: Depoda hic su yok!\n");
                    break;
                }

                int harcanacakSu;
                printf("\nKac birim su kullanilsin? (Mevcut Su: %d): ", su);
                if (!suMiktariOku(&harcanacakSu) || harcanacakSu < 1 || harcanacakSu > su) {
                    printf("[HATA]: Gecersiz su miktari girdiniz. Komut iptal edildi.\n");
                    break;
                }

                enerji -= 5;
                gecerliTurKomutu = 1;

                /* Mod (%) Operatörü ile Artık Su Hesaplama */
                int artikSu = harcanacakSu % 2;
                int donusenSu = harcanacakSu - artikSu;
                int uretilenOksijen = (donusenSu / 2) * 3;

                su -= donusenSu;
                oksijen += uretilenOksijen;

                printf("\n[URETIM]: %d birim su kullanildi (%d birim artik su depoda kaldi).\n", donusenSu, artikSu);
                printf("[URETIM]: +%d birim Oksijen uretildi.\n", uretilenOksijen);
                break;
            }

            case 't':
            case 'T': {
                /* Tamir */
                if (enerji < 10) {
                    printf("\n[YETERSIZ KAYNAK]: Tamir icin en az 10 Enerji gerekli!\n");
                    break;
                }
                if (yedekParca <= 0) {
                    printf("\n[YETERSIZ KAYNAK]: Tamir yapabilmek icin yedek parcaniz yok!\n");
                    break;
                }

                enerji -= 10;
                gecerliTurKomutu = 1;

                /* Hasar Seviyesi (IF - ELSE IF - ELSE Zinciri) */
                if (modulHasari <= 30) {
                    // Hafif Hasar
                    yedekParca -= 1;
                    modulHasari -= 15;
                    printf("\n[TAMIR]: Hafif hasar onarildi. 1 Parca harcandi, modul hasari 15 azaldi.\n");
                } else if (modulHasari <= 70) {
                    // Orta Hasar
                    int harcanan = (yedekParca >= 2) ? 2 : 1;
                    yedekParca -= harcanan;
                    modulHasari -= (harcanan * 15);
                    printf("\n[TAMIR]: Orta seviye hasar onarildi. %d Parca harcandi.\n", harcanan);
                } else {
                    // Agır Hasar
                    int harcanan = (yedekParca >= 3) ? 3 : yedekParca;
                    yedekParca -= harcanan;
                    modulHasari -= (harcanan * 12);
                    printf("\n[TAMIR]: Agir seviye hasar kritikten cikariliyor. %d Parca harcandi.\n", harcanan);
                }
                break;
            }

            case 'd':
            case 'D': {
                /* Durum Raporu (Tur harcamaz) */
                printf("\n========== DETAYLI DURUM RAPORU ==========\n");
                printf("  Saglik       : %d / 100\n", saglik);
                printf("  Enerji       : %d / 100\n", enerji);
                printf("  Oksijen      : %d / 100\n", oksijen);
                printf("  Su Miktari   : %d birim\n", su);
                printf("  Yedek Parca  : %d adet\n", yedekParca);
                printf("  Modul Hasari : %%%d\n", modulHasari);
                printf("  Hava Kilidi  : %s\n", kilitMuhurlu ? "MUHURLU (Guvenli)" : "ACIK (Riskli)");
                printf("  Mevcut Gun   : %d | Toplam Tur: %d\n", gun, turSayaci);
                printf("------------------------------------------\n");

                /* Kritik Uyanlar ve Tek Koşullu Normal Durum (&& Zorunlulugu) */
                if (oksijen < 25) {
                    printf("[UYARI]: Oksijen kritik seviyede! (<25)\n");
                }
                if (saglik < 30) {
                    printf("[UYARI]: Saglik kritik seviyede! (<30)\n");
                }
                if (modulHasari > 70) {
                    printf("[UYARI]: Modul hasari kritik seviyede! (>70%%)\n");
                }

                if (oksijen >= 25 && saglik >= 30 && modulHasari <= 70) {
                    printf("[SISTEM]: TUM SISTEMLER NORMAL\n");
                }
                printf("==========================================\n");
                break;
            }

            case 'u':
            case 'U': {
                /* Uyu / Dinlen */
                gecerliTurKomutu = 1;
                enerji += 25;
                oksijen -= 5;

                /* Önce oksijen kaybı, ardından dinlenmenin sağlık etkisi. */
                if (saglik > 0) {
                    if (oksijen < 20) {
                        saglik -= 10;
                        printf("\n[REST]: Oksijen yetersiz (<20). -10 Saglik.\n");
                    } else {
                        saglik += 15;
                        printf("\n[REST]: +25 Enerji, +15 Saglik, -5 Oksijen.\n");
                    }
                }
                break;
            }

            case 'k':
            case 'K': {
                /* FOR Döngüsü ile Kum Fırtınası Simülasyonu */
                gecerliTurKomutu = 1;
                printf("\n================ KUM FIRTINASI BASLADI (%d TUR) ================\n", firtinaSuresi);

                int toplamHasar = 0;
                int toplamCanKaybi = 0;
                int toplamOksijenKaybi = 0;
                int hasarAlinanTurSayisi = 0;

                for (int t = 1; t <= firtinaSuresi; t++) {
                    int zarFirtina = rand() % 100;
                    int carpan = (t % 2 == 0) ? 2 : 1; // Cift turlarda rüzgar 2 kat siddetlenir

                    printf("Tur %d/%d [Zar: %d | Esikler: saglik %d, sizinti %d, modul %d] ",
                           t, firtinaSuresi, zarFirtina, firtinaSaglikEsigi,
                           firtinaSizintiEsigi, firtinaHasarEsigi);

                    if (zarFirtina < firtinaSaglikEsigi) {
                        // Can Kaybı
                        int kayb = 10 * carpan;
                        if (kayb > saglik) kayb = saglik;
                        saglik -= kayb;
                        toplamCanKaybi += kayb;
                        hasarAlinanTurSayisi++;
                        printf("-> Enkaz firladi! Can kaybi: -%d\n", kayb);
                    } else if (zarFirtina < firtinaSizintiEsigi) {
                        // Oksijen Sızıntısı (CONTINUE Zorunlulugu)
                        if (kilitMuhurlu == 1) {
                            printf("-> Hava kilidi muhurlu oldugu icin sızıntı engellendi! (CONTINUE)\n");
                            continue; // Oksijen kaybı adımı atlanır
                        }
                        int kayb = 15 * carpan;
                        if (kayb > oksijen) kayb = oksijen;
                        oksijen -= kayb;
                        toplamOksijenKaybi += kayb;
                        hasarAlinanTurSayisi++;
                        printf("-> Basınc dusu! Oksijen sızıntisi: -%d\n", kayb);
                    } else if (zarFirtina < firtinaHasarEsigi) {
                        // Modül Hasarı
                        int kayb = 12 * carpan;
                        if (kayb > 100 - modulHasari) kayb = 100 - modulHasari;
                        modulHasari += kayb;
                        toplamHasar += kayb;
                        if (kayb > 0) hasarAlinanTurSayisi++;
                        printf("-> Panel darbe aldi! Modul Hasari: +%%%d\n", kayb);
                    } else {
                        printf("-> Sığınak koruma sağladı (Zararsız atlatıldı).\n");
                    }
                    if (saglik == 0 || oksijen == 0) {
                        printf("[FIRTINA]: Yasamsal veriler sifirlandi. Firtina sona erdi.\n");
                        break;
                    }
                }

                printf("----------------------------------------------------------------\n");
                printf("[FIRTINA OZETI]: Hasar Alinan Tur: %d | Toplam Can Kaybi: %d | Oksijen Kaybi: %d | Modul Hasari: +%%%d\n",
                       hasarAlinanTurSayisi, toplamCanKaybi, toplamOksijenKaybi, toplamHasar);
                printf("================================================================\n");
                break;
            }

            case 'h':
            case 'H': {
                /* Üç yanlış tahminin enerji maliyeti 15'tir. */
                if (enerji < 15) {
                    printf("\n[YETERSIZ KAYNAK]: Hava kilidi icin en az 15 Enerji gerekli.\n");
                    break;
                }
                gecerliTurKomutu = 1;
                char sifre = 'A' + (gun % 26); // Gun sayısından türetilen alfabetik sifre
                char tahmin = ' ';
                int denemeHakki = 3;

                printf("\n[HAVA KILIDI]: Muhurleme Paneli Acildi.\n");
                printf("[IPUCU]: Sifre, alfabede 'A' harfinden bugunun gun sayisi (%d) kadar ilerideki harftir.\n", gun);

                /* Sınırlı Denemeli DO-WHILE Döngüsü */
                do {
                    printf("Kalan Deneme: %d | Sifre Tahmininiz (A-Z): ", denemeHakki);
                    if (scanf(" %c", &tahmin) != 1) {
                        gecerliTurKomutu = 0;
                        oyunDevam = 0;
                        break;
                    }
                    tahmin = (char)toupper((unsigned char)tahmin);

                    if (tahmin == sifre) {
                        kilitMuhurlu = 1;
                        printf("[BASARILI]: Sifre Dogru! Hava kilidi cift contayla muhurlendi.\n");
                        break;
                    } else {
                        denemeHakki--;
                        enerji -= 5;
                        printf("[HATA]: Yanlis Sifre! (-5 Enerji). ");
                        if (tahmin < 'A' || tahmin > 'Z') {
                            printf("A-Z arasinda bir harf giriniz.\n");
                        } else if (tahmin < sifre) {
                            printf("Ipucu: Daha ILERI bir harf giriniz.\n");
                        } else {
                            printf("Ipucu: Daha GERI bir harf giriniz.\n");
                        }
                    }
                } while (tahmin != sifre && denemeHakki > 0);

                if (denemeHakki == 0) {
                    modulHasari += 20;
                    printf("[KILIT BASARISIZ]: 3 hak da tukendi! Sistem zorlandi, Modul Hasari +%%20 artirildi.\n");
                }
                break;
            }

            case 'q':
            case 'Q': {
                printf("\n[CIKIS]: Simulasyondan kendi isteginizle cıkıyorsunuz...\n");
                oyunDevam = 0;
                cikisKomutu = 1;
                turSayaci++;
                break;
            }

            default: {
                printf("\n[HATA]: Tanimsiz komut girdiniz! '%c' gecersizdir. (Süre/Tur harcanmadi)\n", komut);
                break;
            }
        }

        /* Degerlerin Sınırlandırılması (0-100 Arası Clamping) */
        degerleriSinirla(&saglik, &enerji, &oksijen, &modulHasari);

        /* Zaman Akısı ve Gun/Tur Takibi */
        if (gecerliTurKomutu) {
            turSayaci++;
            if (turSayaci % 3 == 0 && gun < 15 && saglik > 0 && oksijen > 0) {
                gun++;
                oksijen -= 8; // Gun bası sabit oksijen tüketimi
                printf("\n>>> YENI GUN BASLADI! (Gun: %d) <<< (-8 Oksijen tuketildi)\n", gun);

                if (modulHasari > 70) {
                    saglik -= 5;
                    printf(">>> [UYARI]: Modul hasari %%70 uzerinde oldugu icin zehirli gaz sızdı (-5 Saglik)! <<<\n");
                }
                degerleriSinirla(&saglik, &enerji, &oksijen, &modulHasari);
            }
        }

        /* Oyun Sonu Koşullarının Kontrolü */
        if (saglik <= 0) {
            printf("\n================ BAD END: HAYATINI KAYBETTIN =================\n");
            printf("Sağlığınız 0'a düştü. Mars atmosferinde yaşam mücadeleniz sona erdi.\n");
            oyunDevam = 0;
        } else if (oksijen <= 0) {
            printf("\n================ BAD END: OKSİJEN TÜKENDİ ==================\n");
            printf("Oksijeniniz bitti. İstasyonda boğularak hayatınızı kaybettiniz.\n");
            oyunDevam = 0;
        } else if (oyunDevam && gun == 15 && turSayaci == 45) {
            printf("\n================ VICTORY: KURTARMA EKİBİ GELDİ! ================\n");
            printf("Tebrikler! 15 gün boyunca Mars'ın zorlu şartlarında hayatta kaldınız!\n");
            oyunDevam = 0;
        }

    } while (oyunDevam);

    /* Oyun Sonu Sonuc Raporu ve Puan Hesaplama */
    int puan = (gun * 100) + saglik + oksijen - modulHasari;

    printf("\n====================================================\n");
    printf("              SIMULASYON SONU RAPORU                \n");
    printf("====================================================\n");
    printf(" Tamamlanan Gun      : %d / 15\n", (turSayaci - cikisKomutu) / 3);
    printf(" Son Gun             : %d\n", gun);
    printf(" Toplam Tamamlanan Tur: %d\n", turSayaci);
    printf(" Son Saglik Durumu   : %d\n", saglik);
    printf(" Son Oksijen Durumu  : %d\n", oksijen);
    printf(" Son Modul Hasari    : %%%d\n", modulHasari);
    printf(" Son Enerji Durumu   : %d\n", enerji);
    printf(" Kalan Su            : %d\n", su);
    printf(" Kalan Yedek Parca   : %d\n", yedekParca);
    printf(" Hava Kilidi         : %s\n", kilitMuhurlu ? "Muhurlu" : "Acik");
    printf("----------------------------------------------------\n");
    printf(" TOPLAM SCORE / PUAN : %d\n", puan);
    printf("====================================================\n");

    return 0;
}
