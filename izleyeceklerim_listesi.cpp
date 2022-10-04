#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

struct kategori_dugum{
	char*isim;
	char*tur;
	char*yonetmeni;
	int suresi;
	struct kategori_dugum*sol;
	struct kategori_dugum*sag;
};
typedef struct kategori_dugum kategori_dugum;
struct kategori_arama_agaci{
    int sayi;
    kategori_dugum*root;
};
typedef struct kategori_arama_agaci kategori_arama_agaci;

struct izleyeceklerim_listesi_dugum{
    kategori_arama_agaci*izleyeceklerim; 
	char*ad;
	int rakam;  
    struct izleyeceklerim_listesi_dugum*sol;
    struct izleyeceklerim_listesi_dugum*sag;
};
typedef struct izleyeceklerim_listesi_dugum izleyeceklerim_listesi_dugum;
struct izleyeceklerim_listesi_arama_agaci {
    int sayi;
    izleyeceklerim_listesi_dugum*root;
};
typedef struct izleyeceklerim_listesi_arama_agaci izleyeceklerim_listesi_arama_agaci;

int sayimi(char*girdi){ //kullanicinin girdigi degerin integer olup olmadigini kontrol eder
    int n = (int)strlen(girdi);
    int i;
    for(i=0;i<n;i++)
        if(!isdigit(girdi[i])) //fonksiyona geçirilen c parametre deðerinin bir rakam olup olmadýðýný kontrol eder
            return 0;
    return 1;
}
void hata_ciktisi(){ //gecersiz ifadeler girildiginde donerek bir sorun olustugunu belirtir
	printf("*******************************************************\n");
    printf("/BIR PROBLEM OLUSTU!/\n");
    printf("/GECERSIZ BIR DEGER GIRDINIZ!/\n");
    printf("/LUTFEN HERHANGI BIR SAYIYA BASIP ENTER'A TIKLAYINIZ!/\n");
    printf("*******************************************************\n");
    char*buf = (char*)malloc(sizeof(char)*16); //malloc fonksiyonuyla beraber dinamik olarak yigindan yer ayirtir
    scanf("%s",buf);
}
kategori_dugum*kategori_dugum_olustur(char*isim, char*tur, char*yonetmeni, int suresi){
    kategori_dugum*temp = (kategori_dugum*)malloc(sizeof(kategori_dugum));
    temp->sol = NULL;
    temp->sag = NULL;
    temp->tur = tur;
    temp->yonetmeni = yonetmeni;
    temp->suresi = suresi;
    temp->isim = isim;
    return temp;
}
izleyeceklerim_listesi_dugum*izleyeceklerim_listesi_dugum_olustur(char*izleyeceklerim_ad,int izleyeceklerim_rakam){ // agac yapisini olusturur
    izleyeceklerim_listesi_dugum*yeni_dugum = (izleyeceklerim_listesi_dugum*)malloc(sizeof(izleyeceklerim_listesi_dugum));
    yeni_dugum->sol = NULL;
    yeni_dugum->sag = NULL;
    yeni_dugum->izleyeceklerim = (kategori_arama_agaci*)malloc(sizeof(kategori_arama_agaci));
    yeni_dugum->izleyeceklerim->sayi = 0;
    yeni_dugum->izleyeceklerim->root = NULL;
    yeni_dugum->ad = izleyeceklerim_ad;
    yeni_dugum->rakam = izleyeceklerim_rakam;
    return yeni_dugum;
}
kategori_dugum*MinDeger(kategori_dugum*izlenecek_dugum){ //Girilen bolumler icin agacin en kucuk dugumu bulmayi saglar 
    kategori_dugum*temp = izlenecek_dugum;
    while(temp->sol!=NULL){
        temp = temp->sol;
    }
    return temp;
}
kategori_dugum*kategori_ekleme_yardim(kategori_dugum*izlenecek_dugum,char*kategori_isim, char*kategori_tur, char*kategori_yonetmeni, int kategori_suresi, int*hata){
    if(izlenecek_dugum){
        int comp = strcmp(izlenecek_dugum->isim, kategori_isim);
        if(comp>0)
            izlenecek_dugum->sol = kategori_ekleme_yardim(izlenecek_dugum->sol,kategori_isim,kategori_tur,kategori_yonetmeni,kategori_suresi,hata);
        else if(comp<0)
            izlenecek_dugum->sag = kategori_ekleme_yardim(izlenecek_dugum->sag,kategori_isim,kategori_tur,kategori_yonetmeni,kategori_suresi,hata);
        else
            *hata = 1;
    }
    else{return kategori_dugum_olustur(kategori_isim,kategori_tur,kategori_yonetmeni,kategori_suresi);}
    return izlenecek_dugum;
}
kategori_dugum*kategori_silme_yardim(kategori_dugum*izlenecek_dugum,char*kategori_isim,int*hata){
    if(izlenecek_dugum){
        int comp = strcmp(izlenecek_dugum->isim, kategori_isim);
        if(comp>0)
            izlenecek_dugum->sol = kategori_silme_yardim(izlenecek_dugum->sol,kategori_isim,hata);
        else if(comp<0)
            izlenecek_dugum->sag = kategori_silme_yardim(izlenecek_dugum->sag,kategori_isim, hata);
        else{
            *hata = 0;
            kategori_dugum*yeni_kategori=NULL;
            kategori_dugum*temp = izlenecek_dugum;
            if(izlenecek_dugum->sol!=NULL&&izlenecek_dugum->sag!=NULL){
                yeni_kategori = MinDeger(izlenecek_dugum->sag);
                yeni_kategori->sol = izlenecek_dugum->sol;
                yeni_kategori->sag = izlenecek_dugum->sag;
                yeni_kategori->sag = kategori_silme_yardim(yeni_kategori->sag,izlenecek_dugum->isim,hata);
            }
            else if(izlenecek_dugum->sol!=NULL){
                yeni_kategori = izlenecek_dugum->sol;
            }
            else if(izlenecek_dugum->sag!=NULL){
                yeni_kategori = izlenecek_dugum->sag;
            }
            free(temp);
            return yeni_kategori;
        }
    }
    return izlenecek_dugum;
}
izleyeceklerim_listesi_dugum*MinDeger_izleyeceklerim(izleyeceklerim_listesi_dugum*izleyeceklerim_dugum){
    izleyeceklerim_listesi_dugum*sonuc=izleyeceklerim_dugum;
    while(sonuc->sol!=NULL){
        sonuc=sonuc->sol;
    }
    return sonuc;
}
int isim_kontrol(izleyeceklerim_listesi_dugum*izleyeceklerim_dugum,char*izleyeceklerim_ad){//girilen bolum isminin daha once girilip girilmedigini kontrol eder . 
    if(izleyeceklerim_dugum){
        if(strcmp(izleyeceklerim_dugum->ad,izleyeceklerim_ad)==0){return 1;}
        return (isim_kontrol(izleyeceklerim_dugum->sol,izleyeceklerim_ad) || isim_kontrol(izleyeceklerim_dugum->sag,izleyeceklerim_ad))? 1 : 0;
    }
    return 0;
}
void kategori_listele(kategori_dugum*izlenecek_dugum){//icerikleri ozelliklerine gore listeler .
    if(izlenecek_dugum){
        kategori_listele(izlenecek_dugum->sol);
        printf("%-36s\n,",izlenecek_dugum->isim);
        printf("%s,",izlenecek_dugum->yonetmeni);
        printf("%s,",izlenecek_dugum->tur);
        printf("%d\n",izlenecek_dugum->suresi);
        kategori_listele(izlenecek_dugum->sag);
    }
}
izleyeceklerim_listesi_dugum*izleyeceklerime_ekle(izleyeceklerim_listesi_dugum*izleyeceklerim_dugum,char*izleyeceklerim_ad,int izleyeceklerim_rakam){//izleyeceklerim listesine  bolum eklemeyi saglar 
    if(izleyeceklerim_dugum){
        if(izleyeceklerim_dugum->rakam>izleyeceklerim_rakam){
            izleyeceklerim_dugum->sol = izleyeceklerime_ekle(izleyeceklerim_dugum->sol,izleyeceklerim_ad,izleyeceklerim_rakam);
        }
        else{
            izleyeceklerim_dugum->sag = izleyeceklerime_ekle(izleyeceklerim_dugum->sag,izleyeceklerim_ad,izleyeceklerim_rakam);
        }
    }
    else{return izleyeceklerim_listesi_dugum_olustur(izleyeceklerim_ad,izleyeceklerim_rakam);}
    return izleyeceklerim_dugum;
}
izleyeceklerim_listesi_dugum*izleyeceklerimden_silme(izleyeceklerim_listesi_dugum*izleyeceklerim_dugum,int*hata,char*izleyeceklerim_ad){
//izleyeceklerim listesinden bolum silmeyi saglar  
    if(izleyeceklerim_dugum){
        if(strcmp(izleyeceklerim_ad,izleyeceklerim_dugum->ad)==0){
            *hata = 0;
            izleyeceklerim_listesi_dugum*temp = izleyeceklerim_dugum;
            izleyeceklerim_listesi_dugum*yeni_dugum = NULL;
            if(izleyeceklerim_dugum->sol!=NULL&&izleyeceklerim_dugum->sag!=NULL){
                yeni_dugum = MinDeger_izleyeceklerim(izleyeceklerim_dugum->sag);
                yeni_dugum->sol = izleyeceklerim_dugum->sol;
                yeni_dugum->sag = izleyeceklerim_dugum->sag;
                yeni_dugum->sag = izleyeceklerimden_silme(yeni_dugum->sag,hata,yeni_dugum->ad);
                return yeni_dugum;
            }
            else if(izleyeceklerim_dugum->sol!=NULL){
                yeni_dugum = izleyeceklerim_dugum->sol;
            }
            else if(izleyeceklerim_dugum->sag!=NULL){
                yeni_dugum = izleyeceklerim_dugum->sag;
            }
            free(temp);
            return yeni_dugum;
        }
        izleyeceklerim_dugum->sol = izleyeceklerimden_silme(izleyeceklerim_dugum->sol,hata,izleyeceklerim_ad);
        izleyeceklerim_dugum->sag = izleyeceklerimden_silme(izleyeceklerim_dugum->sag,hata,izleyeceklerim_ad);
    }
    return izleyeceklerim_dugum;
}
void izleyeceklerim_listele(izleyeceklerim_listesi_dugum*izleyeceklerim_dugum){
    if(izleyeceklerim_dugum){
        izleyeceklerim_listele(izleyeceklerim_dugum->sol);
        printf("%s,",izleyeceklerim_dugum->ad);
        izleyeceklerim_listele(izleyeceklerim_dugum->sag);
    }
}
void kategori_ekleme(izleyeceklerim_listesi_dugum*izleyeceklerim_dugum,char*izleyeceklerim_ad, char*kategori_isim, char*kategori_tur, char*kategori_yonetmeni, int kategori_suresi, int*hata){
  //var olan bolumlere izlenecek icerikleri tur, yonetmen ve sure ile eklemeyi saglar.
    if(izleyeceklerim_dugum){
        if(strcmp(izleyeceklerim_dugum->ad,izleyeceklerim_ad)==0){
            izleyeceklerim_dugum->izleyeceklerim->root = kategori_ekleme_yardim(izleyeceklerim_dugum->izleyeceklerim->root,izleyeceklerim_ad,kategori_tur,kategori_yonetmeni,kategori_suresi,hata);
            return;
        }
        kategori_ekleme(izleyeceklerim_dugum->sol,izleyeceklerim_ad,kategori_isim,kategori_tur,kategori_yonetmeni,kategori_suresi,hata);
        kategori_ekleme(izleyeceklerim_dugum->sag,izleyeceklerim_ad,kategori_isim,kategori_tur,kategori_yonetmeni,kategori_suresi,hata);
    }
}
void kategori_silme(izleyeceklerim_listesi_dugum*izleyeceklerim_dugum,char*izleyeceklerim_ad,char*kategori_isim, int*hata){//silmek istenen icerigi siler 
    if(izleyeceklerim_dugum){
        if(strcmp(izleyeceklerim_dugum->ad,izleyeceklerim_ad)==0){
            izleyeceklerim_dugum->izleyeceklerim->root = kategori_silme_yardim(izleyeceklerim_dugum->izleyeceklerim->root,kategori_isim,hata);
            return;
        }
        kategori_silme(izleyeceklerim_dugum->sol,izleyeceklerim_ad,kategori_isim,hata);
        kategori_silme(izleyeceklerim_dugum->sag,izleyeceklerim_ad,kategori_isim,hata);
    }
}
void tum_kategorileri_listele(izleyeceklerim_listesi_dugum*izleyeceklerim_dugum){//tum izlenecek iceriklerin ozellikleriyle beraber listelenmesini saglar 
    if(izleyeceklerim_dugum){
        tum_kategorileri_listele(izleyeceklerim_dugum->sol);
        kategori_listele(izleyeceklerim_dugum->izleyeceklerim->root);
        tum_kategorileri_listele(izleyeceklerim_dugum->sag);
    }
}
void sure_araligi(kategori_dugum*izlenecek_dugum,int alt_sinir,int ust_sinir,int*hata){//kullanicidan alinan alt ve ust sinirlara gore iceriklerin siralamasini yapan metottur
    if(izlenecek_dugum){
        sure_araligi(izlenecek_dugum->sol,alt_sinir,ust_sinir,hata);
        if(izlenecek_dugum->suresi>=alt_sinir&&izlenecek_dugum->suresi<=ust_sinir){
            printf("*%s,%d,%s\n",izlenecek_dugum->isim,izlenecek_dugum->suresi);
            *hata = 0;
        }
        sure_araligi(izlenecek_dugum->sag,alt_sinir,ust_sinir,hata);
    }
}
void sureye_gore(izleyeceklerim_listesi_dugum*izleyeceklerim_dugum,int alt_sinir,int ust_sinir,int*hata){//sure_araligi metodunu kullanarak  icerikleri suresine gore listeler 
    if(izleyeceklerim_dugum){
        sureye_gore(izleyeceklerim_dugum->sol,alt_sinir,ust_sinir,hata);
        sure_araligi(izleyeceklerim_dugum->izleyeceklerim->root,alt_sinir,ust_sinir,hata);
        sureye_gore(izleyeceklerim_dugum->sag,alt_sinir,ust_sinir,hata);
    }
}

int main(){
    izleyeceklerim_listesi_arama_agaci*izleyeceklerim_listesi = (izleyeceklerim_listesi_arama_agaci*)malloc(sizeof(izleyeceklerim_listesi_arama_agaci));
    izleyeceklerim_listesi->root = NULL;
    izleyeceklerim_listesi->sayi = 0;
    while(1){
        printf("*---------------------------------------------------------------------------*\n");
        printf("                        <Izleyeceklerim Listesi>\n");
        printf("*---------------------------------------------------------------------------*\n");
        printf("*---------------------------------------------------------------------------*\n");
        printf("1.Listeye eklemek istediginiz bolumu secin(Film/Dizi/Belgesel/Yarisma/Anime):\n");
        printf("*---------------------------------------------------------------------------*\n");
        printf("2.Listeden silmek istediginiz bolumu secin(Film/Dizi/Belgesel/Yarsima/Anime):\n");
        printf("*---------------------------------------------------------------------------*\n");
        printf("3.Tum Bolumleri Listele\n");
        printf("*---------------------------------------------------------------------------*\n");
        printf("4.Eklemek istediginiz icerigin adini giriniz:\n");
        printf("*---------------------------------------------------------------------------*\n");
        printf("5.Silmek istediginiz icerigin adini giriniz:\n");
        printf("*---------------------------------------------------------------------------*\n");
        printf("6.Tum izleyeceklerim listesini göster\n");
        printf("*---------------------------------------------------------------------------*\n");
        printf("7.Belirli sure araligindaki tum icerikleri goster\n");
        printf("*---------------------------------------------------------------------------*\n");
        printf("8.Cikis\n");
        printf("*---------------------------------------------------------------------------*\n");
        printf("[!] Lutfen bir secenek giriniz:\n");
        char*secim = (char*)malloc(sizeof(char)*80);
        int numara=0;
        scanf("%s",secim);
        if(!sayimi(secim)){
            hata_ciktisi();
            continue;
        }
        else{
            numara = atoi(secim);
            if(numara<1||numara>10){
                hata_ciktisi();
                continue;
            }
        }
        char*girdiyi_kontrol_et = (char*)malloc(sizeof(char)*30);
        int izleyeceklerim_rakam=0;
        switch(numara){
            case 1:{// eklenecek icerigin bolumunu secme 
                char*izleyeceklerim_ad = (char*)malloc(sizeof(char)*30);
                printf("*********************************************************\n");
                printf("         [EKLENECEK ICERIGIN BOLUMUNU SEC]\n");
                printf("*********************************************************\n");
                printf("LUTFEN EKLEYECEGINIZ ICERIGIN BOLUMUNUN ISMINI GIRINIZ!: \n");
                scanf("%s",izleyeceklerim_ad);
                printf("*********************************************************\n");
                printf("                    [BOLUM EKLE]\n");
                printf("*********************************************************\n");
                printf("LUTFEN SECTIGINIZ BOLUMUN RAKAMINI GIRINIZ:\n");
                printf("Film icin 1, Dizi icin 2, Belgesel icin 3, Yarisma icin 4, Anime icin 5 rakamini tuslayiniz\n");
                scanf("%s",girdiyi_kontrol_et);
                if(!sayimi(girdiyi_kontrol_et)){
                    hata_ciktisi();
                    break;
                }
                else{
                    izleyeceklerim_rakam = atoi(girdiyi_kontrol_et);
                }
               
                if(isim_kontrol(izleyeceklerim_listesi->root,izleyeceklerim_ad)){
                    printf("*************************************************************************\n");
                    printf("                [BOLUM EKLE]\n");
                    printf("*************************************************************************\n");
                    printf("           [!]BOLUM ZATEN EKLI[!]\n");
                    printf("*************************************************************************\n");
                    printf("[LUTFEN MENUYE DONMEK ICIN HERHANGI BIR SAYIYA BASIP ENTER'A TIKLAYIN.]\n");
                    printf("***************************************\n");
                    scanf("%s",girdiyi_kontrol_et);
                    break;
                }
                izleyeceklerim_listesi->root = izleyeceklerime_ekle(izleyeceklerim_listesi->root,izleyeceklerim_ad,izleyeceklerim_rakam);
                izleyeceklerim_listesi->sayi++;
                printf("**************************************************************************************\n");
                printf("                   [BOLUM EKLE]\n");
                printf("**************************************************************************************\n");
                printf("BOLUM:\n");
                printf("%-36s\n",izleyeceklerim_ad);
                printf("[BOLUM EKLENDI. LUTFEN MENUYE DONMEK ICIN HERHANGI BIR SAYIYA BASIP ENTER'A TIKLAYIN.]\n");
                printf("**************************************************************************************\n");
                scanf("%s",girdiyi_kontrol_et);
                break;}
                case 2:{//bolum silme
                printf("*************************************************\n");
                printf("                [BOLUM SIL]\n");
                printf("*************************************************\n");
                printf("LUTFEN SILMEK ISTEDIGINIZ BOLUMUN ISMINI GIRINIZ :\n");
                char*izleyeceklerim_ad = (char*)malloc(sizeof(char)*30);
                scanf("%s",izleyeceklerim_ad);
                int hata = 1;
                izleyeceklerim_listesi->root = izleyeceklerimden_silme(izleyeceklerim_listesi->root,&hata,izleyeceklerim_ad);
                if(hata){
                    printf("***********************************************************************\n");
                    printf("                     	    [BOLUM SIL]\n");
                    printf("***********************************************************************\n");
                    printf("                     [!]BOYLE BIR BOLUM YOK[!]\n");
                    printf("***********************************************************************\n");
                    printf("[LUTFEN MENUYE DONMEK ICIN HERHANGI BIR SAYIYA BASIP ENTER'A TIKLAYIN.]\n");
                    printf("***********************************************************************\n");
                    scanf("%s",girdiyi_kontrol_et);
                }
                else{
                    izleyeceklerim_listesi->sayi--;
                    printf("****************************************************************************************************\n");
                    printf("                          [BOLUM SIL]\n");
                    printf("****************************************************************************************************\n");
                    printf("BOLUM:\n");
                    printf("%-36s\n",izleyeceklerim_ad);
                    printf("[BASARILI BIR SEKILDE SILINDI. LUTFEN MENUYE DONMEK ICIN HERHANGI BIR SAYIYA BASIP ENTER'A TIKLAYIN.]\n");
                    printf("***************************************\n");
                    scanf("%s",girdiyi_kontrol_et);
                }
                break;}
                case 3:{//bolumleri listeleme
                printf("***************************************\n");
                printf("      [BOLUMLERI GORUNTULE]           \n");
                printf("***************************************\n");
                izleyeceklerim_listele(izleyeceklerim_listesi->root);
                if(izleyeceklerim_listesi->root==NULL){
                    printf("--- BIR SEY BULUNAMADI ---\n");
                }
                printf("***************************************\n");
                printf("[GERI DONMEK ICIN HERHANGI BIR SAYIYA BASIP ENTER'A TIKLAYIN.] \n");
                printf("***************************************\n");
                char*buf = (char*)malloc(sizeof(char)*30);
                scanf("%s",buf);
                break;}
                case 4:{//Film/Dizi/Belgesel/Yarisma/Anime ekleme
                char*izleyeceklerim_ad = (char*)malloc(sizeof(izleyeceklerim_ad)*80);
                char*kategori_isim = (char*)malloc(sizeof(char)*80);
                char*kategori_tur = (char*)malloc(sizeof(char)*80);
                char*kategori_yonetmeni = (char*)malloc(sizeof(char)*80);
                int kategori_suresi = 0;
                printf("***************************************\n");
                printf("  [Film/Dizi/Belgesel/Yarisma/Anime EKLE]\n");
                printf("***************************************\n");
                printf("  LUTFEN EKLEMEK ISTEDIGINIZ BOLUMU GIRIN\n");
                scanf(" %s",izleyeceklerim_ad);
                if(isim_kontrol(izleyeceklerim_listesi->root,izleyeceklerim_ad)==0){
                    printf("***************************************\n");
                    printf("    [Film/Dizi/Belgesel/Yarisma/Anime EKLE] \n");
                    printf("***************************************\n");
                    printf("              [!]BOLUM BULUNAMADI[!]              \n");
                    printf("***************************************\n");
                    printf("[GERI DONMEK ICIN HERHANGI BIR SAYIYA BASIP ENTER'A TIKLAYIN.] \n");
                    printf("***************************************\n");
                    scanf("%s",girdiyi_kontrol_et);
                    break;
                }
                
                printf("***************************************\n");
                printf("  [Film/Dizi/Belgesel/Yarisma/Anime EKLE] \n");
                printf("***************************************\n");
                printf("  LUTFEN IZLEYECEGINIZ ICERIGIN ADINI GIRINIZ:  \n");
                scanf(" %s",kategori_isim);
                printf("***************************************\n");
                printf("  LUTFEN IZLEYECEGINIZ ICERIGIN TURUNU GIRINIZ:\n");
                scanf(" %s",kategori_tur);
                printf("***************************************\n");
                printf("  LUTFEN IZLEYECEGINIZ ICERIGIN YONETMENINI GIRINIZ:  \n");
                scanf(" %s",kategori_yonetmeni);
                printf("***************************************\n");
                printf("  LUTFEN IZLEYECEGINIZ ICERIGIN SURE VEYA BOLUM SAYISINI GIRINIZ:\n");
                scanf(" %s",girdiyi_kontrol_et);
                if(!sayimi(girdiyi_kontrol_et)){
                    hata_ciktisi();
                    break;
                }
                else{
                    kategori_suresi = atoi(girdiyi_kontrol_et);
                }
                int hata = 0;
                kategori_ekleme(izleyeceklerim_listesi->root,izleyeceklerim_ad,kategori_isim,kategori_tur,kategori_yonetmeni,kategori_suresi,&hata);
                if(hata){
                    printf("***************************************\n");
                    printf("  [Film/Dizi/Belgesel/Yarisma/Anime EKLE] \n");
                    printf("***************************************\n");
                    printf("  [!]ICERIK COKTAN BULUNUYOR [!]     \n");
                    printf("***************************************\n");
                    printf("  [GERI DONMEK ICIN HERHANGI BIR SAYIYA BASIP ENTER'A TIKLAYIN.] \n");
                    printf("***************************************\n");
                    scanf("%s",girdiyi_kontrol_et);
                }
                else{
                    izleyeceklerim_listesi->root->izleyeceklerim->sayi++;
                    printf("***************************************\n");
                    printf(" [Film/Dizi/Belgesel/Yarisma/Anime EKLE] 	 \n");
                    printf("***************************************\n");
                    printf(" Icerik:                          \n");
                    printf(" %s\n",kategori_isim);
                    printf(" ICERIK EKLENDI. GERI DONMEK ICIN HERHANGI BIR SAYIYA BASIP ENTER'A TIKLAYIN.\n");
                    printf("***************************************\n");
                    scanf("%s",girdiyi_kontrol_et);
                }
                break;}
                case 5:{// Film/Dizi/Belgesel/Yarisma/Anime silme
                char*izleyeceklerim_ad = (char*)malloc(sizeof(izleyeceklerim_ad)*30);
                char*kategori_isim = (char*)malloc(sizeof(char)*30);
                printf("***************************************\n");
                printf("                   [ICERIK SILME]                      \n");
                printf("***************************************\n");
                printf("  LUTFEN SILECEGINIZ ICERIGIN BOLUMUN ISMINI GIRINIZ:\n");
                scanf("%s",izleyeceklerim_ad);
                if(isim_kontrol(izleyeceklerim_listesi->root,izleyeceklerim_ad)==0){
                    printf("***************************************\n");
                    printf("                 [ICERIK SILME]                       \n");
                    printf("***************************************\n");
                    printf("             [!]BOLUM BULUNAMADI[!]     \n");
                    printf("***************************************\n");
                    printf("[GERI DONMEK ICIN HERHANGI BIR SAYIYA BASIP ENTER'A TIKLAYIN.]   \n");
                    printf("***************************************\n");
                    scanf("%s",girdiyi_kontrol_et);
                    break;
                }
                printf("***************************************\n");
                printf("                    [ICERIK SILME]                    \n");
                printf("***************************************\n");
                printf("  LUTFEN SILMEK ISTEDIGINIZ ICERIGIN ADINI GIRINIZ:\n");
                scanf("%s",kategori_isim);
                int hata=1;
                kategori_silme(izleyeceklerim_listesi->root,izleyeceklerim_ad,kategori_isim,&hata);
                if(hata){
                    printf("***************************************\n");
                    printf("               [ICERIK SILME]                      \n");
                    printf("***************************************\n");
                    printf("          [!]ICERIK BULUNAMADI[!] \n");
                    printf("***************************************\n");
                    printf("[GERI DONMEK ICIN HERHANGI BIR SAYIYA BASIP ENTER'A TIKLAYIN.] \n");
                    printf("***************************************\n");
                    scanf("%s",girdiyi_kontrol_et);
                }
                else{
                    izleyeceklerim_listesi->root->izleyeceklerim->sayi--;
                    printf("***************************************\n");
                    printf("*             [ICERIK SILME]                      \n");
                    printf("***************************************\n");
                    printf("ICERIGIN ADI :                       \n");
                    printf("* %-36s*\n",kategori_isim);
                    printf("BASARILI BIR SEKILDE SILINDI \n");
                    printf("[GERI DONMEK ICIN HERHANGI BIR SAYIYA BASIP ENTER'A TIKLAYIN.]\n");
                    printf("***************************************\n");
                    scanf("%s",girdiyi_kontrol_et);
                }
                break;}
                case 6:{//tum iceriklerin listesini goster
                printf("***************************************\n");
                printf("   [TUM ICERIKLERI GOSTER] \n");
                printf("***************************************\n");
                tum_kategorileri_listele(izleyeceklerim_listesi->root);
                if(izleyeceklerim_listesi->root==NULL)
                    printf(" --BIR SEY BULUNAMADI--\n");
                printf("***************************************\n");
                printf("  [GERI DONMEK ICIN HERHANGI BIR SAYIYA BASIP ENTER'A TIKLAYIN.] \n");
                printf("***************************************\n");
                scanf("%s",girdiyi_kontrol_et);
                break;}
                case 7:{//belirli uzunluktaki icerikleri goster 
                int alt_sinir=0;
                int ust_sinir=0;
                printf("***************************************\n");
                printf("[BELIRLI UZUNLUKTAKI ICERIKLERI GOSTER]\n");
                printf("***************************************\n");
                printf("LUTFEN ALT SINIRI GIRINIZ:\n");
                scanf("%s",girdiyi_kontrol_et);
                if(!sayimi(girdiyi_kontrol_et)){
                    hata_ciktisi();
                    break;
                }
                else{
                    alt_sinir = atoi(girdiyi_kontrol_et);
                }
                printf("***************************************\n");
                printf("[BELIRLI UZUNLUKTAKI ICERIKLERI GOSTER]\n");
                printf("***************************************\n");
                printf("LUTFEN UST SINIRI GIRINIZ:\n");
                scanf("%s",girdiyi_kontrol_et);
                if(!sayimi(girdiyi_kontrol_et)){
                    hata_ciktisi();
                    break;
                }
                else{
                    ust_sinir = atoi(girdiyi_kontrol_et);
                }
                printf("***************************************\n");
                printf("[BELIRLI UZUNLUKTAKI ICERIKLERI GOSTER]\n");
                printf("***************************************\n");
                if(alt_sinir>ust_sinir){
                    printf(" [!]UST SINIR DAHA KUCUK OLAMAZ[!] \n");
                }
                else{
                    int hata = 1;
                    sureye_gore(izleyeceklerim_listesi->root,alt_sinir,ust_sinir,&hata);
                    if(hata){
                        printf(" --BIR SEY BULUNAMADI--\n");
                    }
                }
                printf("***************************************\n");
                printf(" [GERI DONMEK ICIN HERHANGI BIR SAYIYA BASIP ENTER'A TIKLAYIN.] \n");
                printf("***************************************\n");
                scanf("%s",girdiyi_kontrol_et);
                break;}
                case 9:{
                return 0;
                break;}
            default:{
                hata_ciktisi();
            }
        }
    }
    return 0;
}
