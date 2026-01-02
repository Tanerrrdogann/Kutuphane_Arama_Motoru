#ifndef BACKEND_H
#define BACKEND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef enum {
    ISLEM_EKLE,
    ISLEM_SIL,
    ISLEM_GUNCELLE
} IslemTuru;

typedef enum {
    KRITER_TF_IDF = 0,
    KRITER_YOK,
    KRITER_BASLIK,
    KRITER_YAZAR,
    KRITER_YIL,
    KRITER_POPULERLIK,
    KRITER_ETIKET
} Kriter;

typedef struct {
    int id;
    char* baslik;
    char* yazar;
    char* isbn;
    int yil;
    char** etiketler;
    int etiket_sayisi;
    float pop;
} Kitap;

typedef struct {
    Kitap** veri;
    int adet;
    int kapasite;
} DiziListe;

typedef struct TrieDugum {
    struct TrieDugum* cocuk[256];
    int son;
    DiziListe* liste;
} TrieDugum;

typedef struct AVL {
    Kitap* kitap;
    struct AVL* sol;
    struct AVL* sag;
    int yukseklik;
} AVL;

typedef struct {
    IslemTuru tur;
    Kitap* etkilenen_kitap;
    Kitap* eski_veri;
} Islem;

typedef struct IslemDugum {
    Islem veri;
    struct IslemDugum* sonraki;
} IslemDugum;

typedef struct {
    IslemDugum* ust;
    int boyut;
} Stack;

typedef struct KuyrukDugum {
    int kitap_id;
    struct KuyrukDugum* sonraki;
} KuyrukDugum;

typedef struct {
    KuyrukDugum *on, *arka;
} Kuyruk;

typedef struct {
    TrieDugum* trie;
    AVL* avl;
    DiziListe* tum_kitaplar;
    Stack* undo_stack;
    Stack* redo_stack;
    int sonraki_id;
} Kutuphane;


Kutuphane* kutuphane_olustur();
void kutuphane_yoket(Kutuphane* k);
void veri_yukle_csv(Kutuphane* k, const char* dosya_adi);
void veri_kaydet_csv(Kutuphane* k, const char* dosya_adi);

Kitap* kitap_olustur(int id, const char* baslik, const char* yazar, const char* isbn, int yil, char** etiketler, int etiket_sayisi, float pop);
void kutuphane_kitap_ekle_veriyapisi(Kutuphane* k, Kitap* kitap);
void kutuphane_yeni_kitap_ekle(Kutuphane* k, const char* baslik, const char* yazar, const char* isbn, int yil, char** etiketler, int etiket_sayisi, float pop);
void kutuphane_kitap_sil(Kutuphane* k, int id);
void kutuphane_kitap_guncelle(Kutuphane* k, int id, Kitap* yeni_veri);
Kitap* id_ile_kitap_getir(Kutuphane* k, int id);
Kitap* kitap_bul_isimle(Kutuphane* k, const char* baslik);

void diziliste_sil(DiziListe* d);
void diziliste_ekle(DiziListe* d, Kitap* k);

void islemi_geri_al(Kutuphane* k);
void islemi_ileri_al(Kutuphane* k);

DiziListe* trie_ara(TrieDugum* kok, const char* onek);
Kitap* avl_bul_id(AVL* kok, int id);
DiziListe* tf_idf_arama_listesi(Kutuphane* k, const char* aranan_kelime);
Kitap* binary_search_baslik(Kitap** dizi, int adet, const char* aranan);

void mergesort_kitap(Kitap** dizi, int sol, int sag, int kriter, int artan_sirada);
void quicksort_kitap(Kitap** dizi, int dusuk, int yuksek, int kriter, int artan_sirada);

DiziListe* detayli_filtrele(DiziListe* kaynak, int min_yil, int max_yil, const char* yazar_parca, const char* baslik_parca, const char* aranan_etiket, float min_pop, float max_pop);
char* en_kisa_yol_bfs_str(Kutuphane* k, int baslangic_id, int hedef_id, int kriter);

#ifdef __cplusplus
}
#endif

#endif
