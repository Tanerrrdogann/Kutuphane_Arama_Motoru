#include "backend.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

char* tr_normalize(const char* s) {
    if(!s) return NULL;

    char* r = malloc(strlen(s) * 2 + 1);
    int i = 0, j = 0;

    while (s[i]) {
        unsigned char c = (unsigned char)s[i];

        if (c == 0xC4 && (unsigned char)s[i+1] == 0xB0) {
            r[j++] = 'i';
            i += 2;
        }
        else if (c == 0xC4 && (unsigned char)s[i+1] == 0xB1) {
            r[j++] = 'i';
            i += 2;
        }
        else if (c == 'I') {
            r[j++] = 'i';
            i++;
        }
        else if (c == 'i') {
            r[j++] = 'i';
            i++;
        }
        else if (c < 128) {
            r[j++] = tolower(c);
            i++;
        }
        else {
            r[j++] = c;
            i++;
        }
    }

    r[j] = '\0';
    return r;
}

int tr_karsilastir(const char* a, const char* b) {
    char* n1 = tr_normalize(a);
    char* n2 = tr_normalize(b);
    int sonuc = strcmp(n1, n2);
    free(n1);
    free(n2);
    return sonuc;
}

char* kopyala(const char* s){
    if(s == NULL) return NULL;
    char* yeni = (char*)malloc(strlen(s) + 1);
    if(yeni == NULL) return NULL;
    strcpy(yeni, s);
    return yeni;
}

int max(int a, int b){
    return (a > b) ? a : b;
}

void swap_kitap(Kitap** a, Kitap** b){
    Kitap* tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

Kitap* kitap_olustur(int id, const char* baslik, const char* yazar,const char* isbn, int yil, char** etiketler, int etiket_sayisi, float pop){
    Kitap* yeni = (Kitap*)malloc(sizeof(Kitap));
    if(yeni == NULL) return NULL;
    int i;

    yeni->id = id;
    yeni->baslik = kopyala(baslik);
    yeni->yazar = kopyala(yazar);
    yeni->isbn = kopyala(isbn);
    yeni->yil = yil;
    yeni->etiket_sayisi = etiket_sayisi;
    yeni->pop = pop;

    if(etiket_sayisi > 0 && etiketler != NULL){
       yeni->etiketler = (char**)malloc(etiket_sayisi * sizeof(char*));
       if(yeni->etiketler == NULL) {
          if(yeni->baslik) free(yeni->baslik);
          if(yeni->yazar) free(yeni->yazar);
          if(yeni->isbn) free(yeni->isbn);
          free(yeni);
          return NULL;
       }

       for(i=0; i<etiket_sayisi; i++){
          yeni->etiketler[i] = kopyala(etiketler[i]);
       }
    } else {
       yeni->etiketler = NULL;
    }
    return yeni;
}

void kitap_sil(Kitap* k){
    if(k == NULL) return;

    if(k->etiketler != NULL){
       for(int i=0; i<k->etiket_sayisi; i++){
          if(k->etiketler[i] != NULL) {
             free(k->etiketler[i]);
             k->etiketler[i] = NULL;
          }
       }
       free(k->etiketler);
       k->etiketler = NULL;
    }

    if(k->baslik) { free(k->baslik); k->baslik = NULL; }
    if(k->yazar) { free(k->yazar); k->yazar = NULL; }
    if(k->isbn) { free(k->isbn); k->isbn = NULL; }

    free(k);
}

DiziListe* diziliste_olustur(){
    DiziListe* d = (DiziListe*)malloc(sizeof(DiziListe));
    if(d == NULL) return NULL;

    d->adet = 0;
    d->kapasite = 4;
    d->veri = (Kitap**)malloc(4 * sizeof(Kitap*));
    if(d->veri == NULL){
       free(d);
       return NULL;
    }
    return d;
}

void diziliste_ekle(DiziListe* d, Kitap* k){
    if(d == NULL) return;

    if(d->adet == d->kapasite){
       int yeni_kapasite = d->kapasite * 2;
       Kitap** temp = (Kitap**)realloc(d->veri, yeni_kapasite * sizeof(Kitap*));

       if(temp == NULL) return;

       d->veri = temp;
       d->kapasite = yeni_kapasite;
    }
    d->veri[d->adet] = k;
    d->adet++;
}

void diziliste_sil(DiziListe* d){
    if(d == NULL) return;
    free(d->veri);
    free(d);
}

void diziliste_cikar_id_ile(DiziListe* d, int id){
    if(d == NULL) return;
    for(int i=0; i < d->adet; i++){
       if(d->veri[i]->id == id){
          for(int j=i; j < d->adet-1; j++){
             d->veri[j] = d->veri[j+1];
          }
          d->adet--;
          return;
       }
    }
}

Stack* stack_olustur(){
    Stack* yeni = (Stack*)malloc(sizeof(Stack));
    if(yeni == NULL) return NULL;
    yeni->ust = NULL;
    yeni->boyut = 0;
    return yeni;
}

void stack_push(Stack* s, Islem islem){
    IslemDugum* yeni = (IslemDugum*)malloc(sizeof(IslemDugum));
    if(yeni == NULL) return;
    yeni->veri = islem;
    yeni->sonraki = s->ust;
    s->ust = yeni;
    s->boyut++;
}

Islem stack_pop(Stack* s) {
    if(s == NULL || s->ust == NULL){
       return (Islem){ .tur = (IslemTuru)-1};
    }

    Islem sakla = s->ust->veri;
    IslemDugum* sil = s->ust;

    s->ust = s->ust->sonraki;
    s->boyut--;

    free(sil);
    return sakla;
}

void stack_yoket(Stack* s) {
    if(s == NULL) return;
    while(s->ust != NULL){
       Islem sil = stack_pop(s);
       if(sil.eski_veri != NULL) kitap_sil(sil.eski_veri);
       if(sil.etkilenen_kitap != NULL) kitap_sil(sil.etkilenen_kitap);
    }
    free(s);
}

void stack_bosalt(Stack* s) {
    if(s == NULL) return;
    while(s->ust != NULL){
       Islem sil = stack_pop(s);
       if(sil.eski_veri != NULL) kitap_sil(sil.eski_veri);
       if(sil.etkilenen_kitap != NULL) kitap_sil(sil.etkilenen_kitap);
    }
}

Kuyruk* kuyruk_olustur(){
    Kuyruk* yeni = (Kuyruk*)malloc(sizeof(Kuyruk));
    if(yeni == NULL) return NULL;
    yeni->on = NULL;
    yeni->arka = NULL;
    return yeni;
}

void kuyruk_ekle(Kuyruk* q, int id){
    KuyrukDugum* yeni = (KuyrukDugum*)malloc(sizeof(KuyrukDugum));
    if(yeni == NULL) return;
    yeni->kitap_id = id;
    yeni->sonraki = NULL;

    if(q->arka == NULL){
       q->arka = yeni;
       q->on = yeni;
       return;
    }
    q->arka->sonraki = yeni;
    q->arka = yeni;
}

int kuyruk_cikar(Kuyruk* q){
    if(q->arka == NULL) return -1;
    KuyrukDugum* sil = q->on;
    int id = sil->kitap_id;
    q->on = q->on->sonraki;
    if(q->on == NULL){
       q->arka = NULL;
    }
    free(sil);
    return id;
}

TrieDugum* trie_dugum_olustur(){
    int i;
    TrieDugum *t = (TrieDugum*)malloc(sizeof(TrieDugum));
    if(t == NULL) return NULL;

    for(i=0; i<256; i++){
       t->cocuk[i] = NULL;
    }
    t->son = 0;
    t->liste = diziliste_olustur();
    if(t->liste == NULL) { free(t); return NULL; }
    return t;
}

void trie_ekle(TrieDugum* kok, const char* anahtar, Kitap* k){
    if (!anahtar) return;
    char* norm_anahtar = tr_normalize(anahtar);
    if (!norm_anahtar) return;

    TrieDugum* t = kok;
    unsigned char c;
    int i;

    for(i=0; norm_anahtar[i]!='\0'; i++){
       c = (unsigned char)norm_anahtar[i];
       if(t->cocuk[c] == NULL){
          t->cocuk[c] = trie_dugum_olustur();
          if(t->cocuk[c] == NULL) { free(norm_anahtar); return; }
       }
       t = t->cocuk[c];
       diziliste_ekle(t->liste, k);
    }
    t->son = 1;
    free(norm_anahtar);
}

DiziListe* trie_ara(TrieDugum* kok, const char* onek){
    if (!onek) return NULL;
    char* norm_onek = tr_normalize(onek);
    if (!norm_onek) return NULL;

    unsigned char c;
    int i;
    TrieDugum* gezgin = kok;

    for(i=0; norm_onek[i]!='\0'; i++){
       c = (unsigned char)norm_onek[i];
       if(gezgin->cocuk[c] == NULL){
          free(norm_onek);
          return NULL;
       }
       gezgin = gezgin->cocuk[c];
    }
    free(norm_onek);

    DiziListe* kopyaListe = diziliste_olustur();
    if(gezgin->liste && gezgin->liste->adet > 0) {
        for(int j=0; j < gezgin->liste->adet; j++) {
            diziliste_ekle(kopyaListe, gezgin->liste->veri[j]);
        }
    }
    return kopyaListe;
}

void trie_yoket(TrieDugum* kok){
    int i;
    if(kok == NULL) return;
    for(i=0; i<256; i++){
       if(kok->cocuk[i] != NULL)
       trie_yoket(kok->cocuk[i]);
    }
    diziliste_sil(kok->liste);
    free(kok);
}

void trie_sil_eleman(TrieDugum* kok, const char* anahtar, int id){
    char* norm_anahtar = tr_normalize(anahtar);
    if(!norm_anahtar) return;

    TrieDugum* sil = kok;
    unsigned char c;
    int i;
    for(i=0; norm_anahtar[i]!='\0'; i++){
       c = (unsigned char)norm_anahtar[i];
       if(sil->cocuk[c] == NULL){
          free(norm_anahtar);
          return;
       }
       sil = sil->cocuk[c];
       if (sil->liste != NULL) {
            diziliste_cikar_id_ile(sil->liste, id);
        }

    }
    diziliste_cikar_id_ile(sil->liste, id);
    if(sil->liste->adet == 0){
       sil->son = 0;
    }
    free(norm_anahtar);
}

int yukseklik(AVL* n){
    if(n == NULL) return 0;
    return n->yukseklik;
}

int denge(AVL* n){
    if(n == NULL) return 0;
    return yukseklik(n->sol) - yukseklik(n->sag);
}

AVL* saga_donus(AVL* n){
    AVL* x = n->sol;
    AVL* y = x->sag;
    x->sag = n;
    n->sol = y;
    n->yukseklik = 1 + max(yukseklik(n->sol), yukseklik(n->sag));
    x->yukseklik = 1 + max(yukseklik(x->sol), yukseklik(x->sag));
    return x;
}

AVL* sola_donus(AVL* n){
    AVL* x = n->sag;
    AVL* y = x->sol;
    x->sol = n;
    n->sag = y;
    n->yukseklik = 1 + max(yukseklik(n->sol), yukseklik(n->sag));
    x->yukseklik = 1 + max(yukseklik(x->sol), yukseklik(x->sag));
    return x;
}

AVL* avl_ekle(AVL* kok, Kitap* k){
    if(kok == NULL){
       AVL* yeni = (AVL*)malloc(sizeof(AVL));
       if(yeni == NULL) return NULL;
       yeni->kitap = k;
       yeni->sol = NULL;
       yeni->sag = NULL;
       yeni->yukseklik = 1;
       return yeni;
    }
    if(k->id < kok->kitap->id){
       kok->sol = avl_ekle(kok->sol, k);
    }
    else if(k->id > kok->kitap->id){
       kok->sag = avl_ekle(kok->sag, k);
    }
    else{
       return kok;
    }
    kok->yukseklik = 1 + max(yukseklik(kok->sol), yukseklik(kok->sag));
    int d = denge(kok);
    if(d > 1 && k->id < kok->sol->kitap->id)
       return saga_donus(kok);
    if(d < -1 && k->id > kok->sag->kitap->id)
       return sola_donus(kok);
    if(d > 1 && k->id > kok->sol->kitap->id){
       kok->sol = sola_donus(kok->sol);
       return saga_donus(kok);
    }
    if(d < -1 && k->id < kok->sag->kitap->id){
       kok->sag = saga_donus(kok->sag);
       return sola_donus(kok);
    }
    return kok;
}

Kitap* avl_bul_id(AVL* kok, int id){
    if(kok == NULL) return NULL;
    AVL* bul = kok;
    while(bul != NULL){
       if(bul->kitap->id == id){
          return bul->kitap;
       } else if(id < bul->kitap->id){
          bul = bul->sol;
       } else {
          bul = bul->sag;
       }
    }
    return NULL;
}

AVL* avl_min_deger_dugumu(AVL* dugum) {
    if(dugum == NULL) return NULL;
    AVL* bul = dugum;
    while(bul->sol != NULL){
       bul = bul->sol;
    }
    return bul;
}

AVL* avl_sil_id(AVL* kok, int id) {
    if(kok == NULL) return NULL;

    if(id < kok->kitap->id){
       kok->sol = avl_sil_id(kok->sol, id);
    } else if(id > kok->kitap->id){
       kok->sag = avl_sil_id(kok->sag, id);
    } else {
       if(kok->sol == NULL){
          AVL* gonder = kok->sag;
          free(kok);
          return gonder;
       } else if(kok->sag == NULL){
          AVL* gonder = kok->sol;
          free(kok);
          return gonder;
       } else {
          AVL* sag_min = avl_min_deger_dugumu(kok->sag);
          kok->kitap = sag_min->kitap;
          kok->sag = avl_sil_id(kok->sag, sag_min->kitap->id);
       }
    }

    kok->yukseklik = 1 + max(yukseklik(kok->sol), yukseklik(kok->sag));
    int d = denge(kok);

    if(d > 1){
       if(denge(kok->sol) >= 0) return saga_donus(kok);
       if(denge(kok->sol) < 0){
          kok->sol = sola_donus(kok->sol);
          return saga_donus(kok);
       }
    }
    if(d < -1){
       if(denge(kok->sag) <= 0) return sola_donus(kok);
       if(denge(kok->sag) > 0){
          kok->sag = saga_donus(kok->sag);
          return sola_donus(kok);
       }
    }
    return kok;
}

void avl_yoket(AVL* kok){
    if(kok == NULL) return;
    avl_yoket(kok->sol);
    avl_yoket(kok->sag);
    free(kok);
}

Kutuphane* kutuphane_olustur(){
    Kutuphane* yeni = (Kutuphane*)malloc(sizeof(Kutuphane));
    if(yeni == NULL) return NULL;
    yeni->trie = trie_dugum_olustur();
    yeni->avl = NULL;
    yeni->tum_kitaplar = diziliste_olustur();
    yeni->undo_stack = stack_olustur();
    yeni->redo_stack = stack_olustur();
    yeni->sonraki_id = 1;
    return yeni;
}

void kutuphane_kitap_ekle_veriyapisi(Kutuphane* k, Kitap* kitap) {
    trie_ekle(k->trie, kitap->baslik, kitap);
    k->avl = avl_ekle(k->avl, kitap);
    diziliste_ekle(k->tum_kitaplar, kitap);
}

Kitap* id_ile_kitap_getir(Kutuphane* k, int id){
    return avl_bul_id(k->avl, id);
}

Kitap* kitap_bul_isimle(Kutuphane* k, const char* baslik)
{
    if(!k || !baslik) return NULL;

    DiziListe* adaylar = trie_ara(k->trie, baslik);
    if(!adaylar) return NULL;

    char* norm_baslik = tr_normalize(baslik);

    for(int i = 0; i < adaylar->adet; i++){
        Kitap* aday = adaylar->veri[i];
        if(!aday || !aday->baslik) continue;

        char* norm_aday = tr_normalize(aday->baslik);

        if(strcmp(norm_aday, norm_baslik) == 0){
            free(norm_aday);
            free(norm_baslik);

            Kitap* sonuc = aday;
            diziliste_sil(adaylar);
            return sonuc;
        }

        free(norm_aday);
    }

    diziliste_sil(adaylar);
    free(norm_baslik);
    return NULL;
}


void kutuphane_yeni_kitap_ekle(Kutuphane* k, const char* baslik, const char* yazar, const char* isbn, int yil, char** etiketler, int etiket_sayisi, float pop) {
    Kitap* yeni = kitap_olustur(k->sonraki_id, baslik, yazar, isbn, yil, etiketler, etiket_sayisi, pop);
    if(yeni == NULL) return;

    k->sonraki_id++;

    Islem islem;
    islem.tur = ISLEM_EKLE;
    islem.etkilenen_kitap = yeni;
    islem.eski_veri = NULL;
    stack_push(k->undo_stack, islem);
    stack_bosalt(k->redo_stack);

    kutuphane_kitap_ekle_veriyapisi(k, yeni);
}

void kutuphane_kitap_sil(Kutuphane* k, int id) {
    Kitap* sil = id_ile_kitap_getir(k, id);
    if(sil == NULL) return;

    Kitap* kopya = kitap_olustur(sil->id, sil->baslik, sil->yazar, sil->isbn, sil->yil, sil->etiketler, sil->etiket_sayisi, sil->pop);

    k->avl = avl_sil_id(k->avl, id);
    diziliste_cikar_id_ile(k->tum_kitaplar, id);
    trie_sil_eleman(k->trie, sil->baslik, id);

    Islem islem;
    islem.tur = ISLEM_SIL;
    islem.etkilenen_kitap = NULL;
    islem.eski_veri = kopya;
    stack_push(k->undo_stack, islem);
    stack_bosalt(k->redo_stack);

    kitap_sil(sil);
}

void kutuphane_kitap_guncelle(Kutuphane* k, int id, Kitap* yeni_veri){
    Kitap* sil = id_ile_kitap_getir(k, id);
    if(sil == NULL) return;

    Kitap* kopya = kitap_olustur(sil->id, sil->baslik, sil->yazar, sil->isbn, sil->yil, sil->etiketler, sil->etiket_sayisi, sil->pop);

    k->avl = avl_sil_id(k->avl, id);
    diziliste_cikar_id_ile(k->tum_kitaplar, id);
    trie_sil_eleman(k->trie, sil->baslik, id);

    yeni_veri->id = id;
    kutuphane_kitap_ekle_veriyapisi(k, yeni_veri);

    Islem islem;
    islem.tur = ISLEM_GUNCELLE;
    islem.etkilenen_kitap = yeni_veri;
    islem.eski_veri = kopya;
    stack_push(k->undo_stack, islem);
    stack_bosalt(k->redo_stack);

    kitap_sil(sil);
}

void kutuphane_yoket(Kutuphane* k){
    if(k==NULL) return;
    for(int i=0; i < k->tum_kitaplar->adet; i++){
       kitap_sil(k->tum_kitaplar->veri[i]);
    }
    trie_yoket(k->trie);
    avl_yoket(k->avl);
    diziliste_sil(k->tum_kitaplar);
    stack_yoket(k->undo_stack);
    stack_yoket(k->redo_stack);
    free(k);
}

void islemi_geri_al(Kutuphane* k){
    if(k->undo_stack == NULL || k->undo_stack->ust == NULL) return;

    Islem islem = stack_pop(k->undo_stack);
    Islem geri_al_icin;

    switch(islem.tur){
        case ISLEM_EKLE: {
             Kitap* silinecek = id_ile_kitap_getir(k, islem.etkilenen_kitap->id);

             if(silinecek){
                 Kitap* kopya = kitap_olustur(silinecek->id, silinecek->baslik, silinecek->yazar, silinecek->isbn, silinecek->yil, silinecek->etiketler, silinecek->etiket_sayisi, silinecek->pop);

                 geri_al_icin.tur = ISLEM_EKLE;
                 geri_al_icin.etkilenen_kitap = kopya;
                 geri_al_icin.eski_veri = NULL;
                 stack_push(k->redo_stack, geri_al_icin);

                 diziliste_cikar_id_ile(k->tum_kitaplar, silinecek->id);
                 k->avl = avl_sil_id(k->avl, silinecek->id);
                 trie_sil_eleman(k->trie, silinecek->baslik, silinecek->id);

                 kitap_sil(silinecek);
             }
             break;
        }
        case ISLEM_SIL: {
             Kitap* eklenecek_veri = islem.eski_veri;

             if(eklenecek_veri){
                 Kitap* yeni_kitap = kitap_olustur(eklenecek_veri->id, eklenecek_veri->baslik, eklenecek_veri->yazar, eklenecek_veri->isbn, eklenecek_veri->yil, eklenecek_veri->etiketler, eklenecek_veri->etiket_sayisi, eklenecek_veri->pop);
                 kutuphane_kitap_ekle_veriyapisi(k, yeni_kitap);

                 Kitap* redo_kopya = kitap_olustur(eklenecek_veri->id, eklenecek_veri->baslik, eklenecek_veri->yazar, eklenecek_veri->isbn, eklenecek_veri->yil, eklenecek_veri->etiketler, eklenecek_veri->etiket_sayisi, eklenecek_veri->pop);

                 geri_al_icin.tur = ISLEM_SIL;
                 geri_al_icin.etkilenen_kitap = NULL;
                 geri_al_icin.eski_veri = redo_kopya;
                 stack_push(k->redo_stack, geri_al_icin);

                 kitap_sil(eklenecek_veri);
             }
             break;
        }
        case ISLEM_GUNCELLE: {
             Kitap* su_anki_hali = id_ile_kitap_getir(k, islem.etkilenen_kitap->id);
             Kitap* eski_hali = islem.eski_veri;

             if(su_anki_hali && eski_hali){
                 Kitap* redo_kopya = kitap_olustur(su_anki_hali->id, su_anki_hali->baslik, su_anki_hali->yazar, su_anki_hali->isbn, su_anki_hali->yil, su_anki_hali->etiketler, su_anki_hali->etiket_sayisi, su_anki_hali->pop);

                 diziliste_cikar_id_ile(k->tum_kitaplar, su_anki_hali->id);
                 k->avl = avl_sil_id(k->avl, su_anki_hali->id);
                 trie_sil_eleman(k->trie, su_anki_hali->baslik, su_anki_hali->id);
                 kitap_sil(su_anki_hali);

                 Kitap* geri_yuklenen = kitap_olustur(eski_hali->id, eski_hali->baslik, eski_hali->yazar, eski_hali->isbn, eski_hali->yil, eski_hali->etiketler, eski_hali->etiket_sayisi, eski_hali->pop);
                 kutuphane_kitap_ekle_veriyapisi(k, geri_yuklenen);

                 geri_al_icin.tur = ISLEM_GUNCELLE;
                 geri_al_icin.etkilenen_kitap = geri_yuklenen;
                 geri_al_icin.eski_veri = redo_kopya;
                 stack_push(k->redo_stack, geri_al_icin);

                 kitap_sil(eski_hali);
             }
             break;
        }
    }
}

void islemi_ileri_al(Kutuphane* k){
    if(k->redo_stack == NULL || k->redo_stack->ust == NULL) return;

    Islem islem = stack_pop(k->redo_stack);
    Islem ileri_al_icin;

    switch(islem.tur){
        case ISLEM_EKLE: {
             Kitap* eklenecek = islem.etkilenen_kitap;

             if(eklenecek){
                 Kitap* yeni_kitap = kitap_olustur(eklenecek->id, eklenecek->baslik, eklenecek->yazar, eklenecek->isbn, eklenecek->yil, eklenecek->etiketler, eklenecek->etiket_sayisi, eklenecek->pop);
                 kutuphane_kitap_ekle_veriyapisi(k, yeni_kitap);

                 ileri_al_icin.tur = ISLEM_EKLE;
                 ileri_al_icin.etkilenen_kitap = yeni_kitap;
                 ileri_al_icin.eski_veri = NULL;
                 stack_push(k->undo_stack, ileri_al_icin);

                 kitap_sil(eklenecek);
             }
             break;
        }
        case ISLEM_SIL: {
             Kitap* silinecek = id_ile_kitap_getir(k, islem.eski_veri->id);

             if(silinecek){
                 Kitap* undo_kopya = kitap_olustur(silinecek->id, silinecek->baslik, silinecek->yazar, silinecek->isbn, silinecek->yil, silinecek->etiketler, silinecek->etiket_sayisi, silinecek->pop);

                 diziliste_cikar_id_ile(k->tum_kitaplar, silinecek->id);
                 k->avl = avl_sil_id(k->avl, silinecek->id);
                 trie_sil_eleman(k->trie, silinecek->baslik, silinecek->id);
                 kitap_sil(silinecek);

                 ileri_al_icin.tur = ISLEM_SIL;
                 ileri_al_icin.etkilenen_kitap = NULL;
                 ileri_al_icin.eski_veri = undo_kopya;
                 stack_push(k->undo_stack, ileri_al_icin);

                 if(islem.eski_veri) kitap_sil(islem.eski_veri);
             }
             break;
        }
        case ISLEM_GUNCELLE: {
             Kitap* su_anki = id_ile_kitap_getir(k, islem.etkilenen_kitap->id);
             Kitap* yeni_hali = islem.eski_veri;

             if(su_anki && yeni_hali){
                 Kitap* undo_kopya = kitap_olustur(su_anki->id, su_anki->baslik, su_anki->yazar, su_anki->isbn, su_anki->yil, su_anki->etiketler, su_anki->etiket_sayisi, su_anki->pop);

                 diziliste_cikar_id_ile(k->tum_kitaplar, su_anki->id);
                 k->avl = avl_sil_id(k->avl, su_anki->id);
                 trie_sil_eleman(k->trie, su_anki->baslik, su_anki->id);
                 kitap_sil(su_anki);

                 Kitap* guncel_kitap = kitap_olustur(yeni_hali->id, yeni_hali->baslik, yeni_hali->yazar, yeni_hali->isbn, yeni_hali->yil, yeni_hali->etiketler, yeni_hali->etiket_sayisi, yeni_hali->pop);
                 kutuphane_kitap_ekle_veriyapisi(k, guncel_kitap);

                 ileri_al_icin.tur = ISLEM_GUNCELLE;
                 ileri_al_icin.etkilenen_kitap = guncel_kitap;
                 ileri_al_icin.eski_veri = undo_kopya;
                 stack_push(k->undo_stack, ileri_al_icin);

                 kitap_sil(yeni_hali);
             }
             break;
        }
    }
}

void veri_kaydet_csv(Kutuphane* k, const char* dosya_adi){
    FILE* f = fopen(dosya_adi, "w");
    if(f == NULL) return;
    Kitap** x = k->tum_kitaplar->veri;
    for(int i = 0; i < k->tum_kitaplar->adet; i++){
       fprintf(f,"%d,%s,%s,%s,%d,%d,%.2f,", x[i]->id, x[i]->baslik, x[i]->yazar, x[i]->isbn, x[i]->yil, x[i]->etiket_sayisi, x[i]->pop);

       if(x[i]->etiket_sayisi > 0) {
           for(int j = 0; j < x[i]->etiket_sayisi; j++){
               fprintf(f, "%s", x[i]->etiketler[j]);
               if(j < x[i]->etiket_sayisi - 1) fprintf(f, "|");
           }
       } else {
           fprintf(f, "NULL");
       }
       fprintf(f, "\n");
    }
    fclose(f);
}

void veri_yukle_csv(Kutuphane* k, const char* dosya_adi){
    FILE* f = fopen(dosya_adi, "r");
    if(f == NULL) return;

    char tampon[4096];
    char* parca;

    int id;
    char baslik[256];
    char yazar[256];
    char isbn[64];
    int yil;
    char* etiketler[20];
    int etiket_sayisi;
    float pop;

    while(fgets(tampon, sizeof(tampon), f) != NULL){
       tampon[strcspn(tampon, "\r\n")] = 0;

       parca = strtok(tampon, ",");
       if(!parca) continue;
       id = atoi(parca);

       parca = strtok(NULL, ",");
       if(!parca) strcpy(baslik, "Bilinmiyor");
       else { strncpy(baslik, parca, 255); baslik[255] = '\0'; }

       parca = strtok(NULL, ",");
       if(!parca) strcpy(yazar, "Bilinmiyor");
       else { strncpy(yazar, parca, 255); yazar[255] = '\0'; }

       parca = strtok(NULL, ",");
       if(!parca) strcpy(isbn, "-");
       else { strncpy(isbn, parca, 63); isbn[63] = '\0'; }

       parca = strtok(NULL, ",");
       yil = (parca) ? atoi(parca) : 0;

       parca = strtok(NULL, ",");
       etiket_sayisi = (parca) ? atoi(parca) : 0;

       parca = strtok(NULL, ",");
       pop = (parca) ? atof(parca) : 0.0f;

       parca = strtok(NULL, "\n");

       if(etiket_sayisi > 0 && parca != NULL && strcmp(parca, "NULL") != 0){
          char* etiket_ptr = strtok(parca, "|");
          int i = 0;
          while(etiket_ptr != NULL && i < etiket_sayisi){
              etiketler[i] = etiket_ptr;
              etiket_ptr = strtok(NULL, "|");
              i++;
          }
          etiket_sayisi = i;
       } else {
           etiket_sayisi = 0;
       }

       Kitap* yeni = kitap_olustur(id, baslik, yazar, isbn, yil, etiketler, etiket_sayisi, pop);
       if(yeni != NULL){
          kutuphane_kitap_ekle_veriyapisi(k, yeni);
          if(id >= k->sonraki_id) k->sonraki_id = id + 1;
       }
    }
    fclose(f);
}

void merge(Kitap** dizi, int sol, int orta, int sag, int kriter, int artan_sirada){
    int i, j, k;
    int n1 = orta - sol + 1;
    int n2 = sag - orta;
    Kitap** L = (Kitap**)malloc(n1 * sizeof(Kitap*));
    Kitap** R = (Kitap**)malloc(n2 * sizeof(Kitap*));
    if (L == NULL || R == NULL) return;

    for (i=0; i < n1; i++) L[i] = dizi[sol + i];
    for (j=0; j < n2; j++) R[j] = dizi[orta + 1 + j];

    i = 0; j = 0; k = sol;
    while (i < n1 && j < n2) {
       int sol_oncelikli = 0;
       int karsilastirma = 0;

       if (kriter == KRITER_BASLIK) karsilastirma = tr_karsilastir(L[i]->baslik, R[j]->baslik);
       else if (kriter == KRITER_YAZAR) karsilastirma = tr_karsilastir(L[i]->yazar, R[j]->yazar);
       else if (kriter == KRITER_YIL) karsilastirma = (L[i]->yil < R[j]->yil) ? -1 : (L[i]->yil > R[j]->yil ? 1 : 0);
       else if (kriter == KRITER_POPULERLIK) karsilastirma = (L[i]->pop < R[j]->pop) ? -1 : (L[i]->pop > R[j]->pop ? 1 : 0);
       else if (kriter == KRITER_ETIKET) karsilastirma = (L[i]->etiket_sayisi < R[j]->etiket_sayisi) ? -1 : (L[i]->etiket_sayisi > R[j]->etiket_sayisi ? 1 : 0);

       if (artan_sirada) {
           if (karsilastirma < 0) sol_oncelikli = 1;
       } else {
           if (karsilastirma > 0) sol_oncelikli = 1;
       }

       if (sol_oncelikli) { dizi[k] = L[i]; i++; }
       else { dizi[k] = R[j]; j++; }
       k++;
    }
    while (i < n1) { dizi[k] = L[i]; i++; k++; }
    while (j < n2) { dizi[k] = R[j]; j++; k++; }
    free(L); free(R);
}

void mergesort_kitap(Kitap** dizi, int sol, int sag, int kriter, int artan_sirada){
    if(sol < sag){
       int orta = sol + (sag - sol) / 2;
       mergesort_kitap(dizi, sol, orta, kriter, artan_sirada);
       mergesort_kitap(dizi, orta+1, sag, kriter, artan_sirada);
       merge(dizi, sol, orta, sag, kriter, artan_sirada);
    }
}

int partition(Kitap** dizi, int dusuk, int yuksek, int kriter, int artan_sirada){
    Kitap* pivot = dizi[yuksek];
    int i = dusuk - 1, j;
    for(j=dusuk; j<yuksek; j++){
       int condition = 0;
       int karsilastirma = 0;

       if (kriter == KRITER_BASLIK) karsilastirma = tr_karsilastir(dizi[j]->baslik, pivot->baslik);
       else if (kriter == KRITER_YAZAR) karsilastirma = tr_karsilastir(dizi[j]->yazar, pivot->yazar);
       else if (kriter == KRITER_YIL) karsilastirma = (dizi[j]->yil < pivot->yil) ? -1 : (dizi[j]->yil > pivot->yil ? 1 : 0);
       else if (kriter == KRITER_POPULERLIK) karsilastirma = (dizi[j]->pop < pivot->pop) ? -1 : (dizi[j]->pop > pivot->pop ? 1 : 0);
       else if (kriter == KRITER_ETIKET) karsilastirma = (dizi[j]->etiket_sayisi < pivot->etiket_sayisi) ? -1 : (dizi[j]->etiket_sayisi > pivot->etiket_sayisi ? 1 : 0);

       if (artan_sirada) { if (karsilastirma < 0) condition = 1; }
       else { if (karsilastirma > 0) condition = 1; }

       if(condition){
          i++;
          swap_kitap(&dizi[i], &dizi[j]);
       }
    }
    swap_kitap(&dizi[yuksek], &dizi[i+1]);
    return i+1;
}

void quicksort_kitap(Kitap** dizi, int dusuk, int yuksek, int kriter, int artan_sirada){
    if(yuksek > dusuk){
       int pi = partition(dizi, dusuk, yuksek, kriter, artan_sirada);
       quicksort_kitap(dizi, dusuk, pi-1, kriter, artan_sirada);
       quicksort_kitap(dizi, pi+1, yuksek, kriter, artan_sirada);
    }
}

typedef struct { Kitap* k; double skor; } SkorluKitap;
int karsilastir_skor(const void* a, const void* b) {
    SkorluKitap* k1 = (SkorluKitap*)a;
    SkorluKitap* k2 = (SkorluKitap*)b;
    if (k2->skor > k1->skor) return 1;
    if (k2->skor < k1->skor) return -1;
    return 0;
}

int kelime_kac_kere_geciyor(Kitap* k, const char* norm_aranan) {
    int sayac = 0;
    char* p;

    if(k->baslik) {
        char* norm_baslik = tr_normalize(k->baslik);
        if(norm_baslik) {
            p = norm_baslik;
            while((p = strstr(p, norm_aranan)) != NULL) {
                sayac += 5;
                p += strlen(norm_aranan);
            }
            free(norm_baslik);
        }
    }

    if(k->yazar) {
        char* norm_yazar = tr_normalize(k->yazar);
        if(norm_yazar) {
            p = norm_yazar;
            while((p = strstr(p, norm_aranan)) != NULL) {
                sayac += 3;
                p += strlen(norm_aranan);
            }
            free(norm_yazar);
        }
    }

    if(k->etiketler) {
        for(int i=0; i<k->etiket_sayisi; i++) {
            if(k->etiketler[i]) {
                char* norm_etiket = tr_normalize(k->etiketler[i]);
                if(norm_etiket) {
                    p = norm_etiket;
                    while((p = strstr(p, norm_aranan)) != NULL) {
                        sayac += 2;
                        p += strlen(norm_aranan);
                    }
                    free(norm_etiket);
                }
            }
        }
    }
    return sayac;
}

DiziListe* tf_idf_arama_listesi(Kutuphane* k, const char* aranan_kelime){
    if (k == NULL || aranan_kelime == NULL) return NULL;

    char* norm_aranan = tr_normalize(aranan_kelime);
    if(!norm_aranan) return NULL;

    int i;
    int toplam = k->tum_kitaplar->adet;
    int iceren = 0;

    for(i=0; i<toplam; i++){
        if(kelime_kac_kere_geciyor(k->tum_kitaplar->veri[i], norm_aranan) > 0){
           iceren++;
        }
    }
    if(iceren == 0) { free(norm_aranan); return NULL; }

    double idf = log((double)toplam / (double)iceren);
    SkorluKitap* liste = (SkorluKitap*)malloc(toplam * sizeof(SkorluKitap));
    int sayac = 0;

    for(i=0; i<toplam; i++){
        int tf = kelime_kac_kere_geciyor(k->tum_kitaplar->veri[i], norm_aranan);

        if(tf > 0){
           liste[sayac].k = k->tum_kitaplar->veri[i];
           liste[sayac].skor = tf * idf;
           sayac++;
        }
    }

    qsort(liste, sayac, sizeof(SkorluKitap), karsilastir_skor);

    DiziListe* sonuc = diziliste_olustur();
    for(i=0; i<sayac; i++){
        diziliste_ekle(sonuc, liste[i].k);
    }
    free(liste);
    free(norm_aranan);
    return sonuc;
}

Kitap* binary_search_baslik(Kitap** dizi, int adet, const char* aranan) {
    int sol = 0;
    int sag = adet - 1;
    char* norm_aranan = tr_normalize(aranan);

    while (sol <= sag) {
       int orta = sol + (sag - sol) / 2;

       char* norm_orta = tr_normalize(dizi[orta]->baslik);
       int kiyas = strcmp(norm_orta, norm_aranan);
       free(norm_orta);

       if (kiyas == 0) {
           free(norm_aranan);
           return dizi[orta];
       }

       if (kiyas < 0)
           sol = orta + 1;
       else
           sag = orta - 1;
    }
    free(norm_aranan);
    return NULL;
}

char* en_kisa_yol_bfs_str(Kutuphane* k, int baslangic_id, int hedef_id, int kriter){
    int* bak = (int*)calloc(k->tum_kitaplar->adet + 100, sizeof(int));
    int* onceki = (int*)malloc((k->tum_kitaplar->adet + 100) * sizeof(int));
    int i, baslangic_index=-1, hedef_index=-1, baglanti_var=0, a, b, bulundu=0;

    for(i=0; i < k->tum_kitaplar->adet; i++){
       if(k->tum_kitaplar->veri[i]->id == baslangic_id) baslangic_index = i;
       if(k->tum_kitaplar->veri[i]->id == hedef_id) hedef_index = i;
    }

    char* sonuc_str = (char*)malloc(2048);
    strcpy(sonuc_str, "");

    if(hedef_index == -1 || baslangic_index == -1){
       strcpy(sonuc_str, "Hata: Baslangic veya hedef Kitap ID bulunamadi.");
       free(bak); free(onceki); return sonuc_str;
    }

    Kuyruk* q = kuyruk_olustur();
    kuyruk_ekle(q, baslangic_index);
    bak[baslangic_index] = 1;
    onceki[baslangic_index] = -1;

    while(q->on != NULL){
       int simdiki = kuyruk_cikar(q);
       if(simdiki == hedef_index){
          bulundu = 1;
          break;
       }
       for(int j=0; j < k->tum_kitaplar->adet; j++){
          baglanti_var = 0;

          switch(kriter){
             case(KRITER_BASLIK): {
                if(tr_karsilastir(k->tum_kitaplar->veri[j]->baslik, k->tum_kitaplar->veri[simdiki]->baslik) == 0 && bak[j] == 0) baglanti_var = 1;
                break;
             }
             case(KRITER_POPULERLIK):
                if(k->tum_kitaplar->veri[j]->pop == k->tum_kitaplar->veri[simdiki]->pop && bak[j] == 0) baglanti_var = 1;
                break;
             case(KRITER_YAZAR): {
                if(tr_karsilastir(k->tum_kitaplar->veri[j]->yazar, k->tum_kitaplar->veri[simdiki]->yazar) == 0 && bak[j] == 0) baglanti_var = 1;
                break;
             }
             case(KRITER_YIL):
                if(k->tum_kitaplar->veri[j]->yil == k->tum_kitaplar->veri[simdiki]->yil && bak[j] == 0) baglanti_var = 1;
                break;
             case(KRITER_ETIKET):
                if(k->tum_kitaplar->veri[j]->etiketler && k->tum_kitaplar->veri[simdiki]->etiketler) {
                    for(a=0; a < k->tum_kitaplar->veri[j]->etiket_sayisi; a++){
                       for(b=0; b < k->tum_kitaplar->veri[simdiki]->etiket_sayisi; b++){
                             if(tr_karsilastir(k->tum_kitaplar->veri[j]->etiketler[a], k->tum_kitaplar->veri[simdiki]->etiketler[b]) == 0 && bak[j] == 0){
                             baglanti_var = 1; break;
                          }
                       }
                       if(baglanti_var == 1) break;
                    }
                }
                break;
             default: break;
          }

          if(baglanti_var == 1){
             bak[j] = 1;
             onceki[j] = simdiki;
             kuyruk_ekle(q, j);
          }
       }
    }

    if(bulundu == 1){
       int gezgin = hedef_index;
       strcat(sonuc_str, "BAĞLANTI YOLU BULUNDU:\n\n");
       int yol_indexleri[100];
       int yol_uzunlugu = 0;
       while(gezgin != -1){
          yol_indexleri[yol_uzunlugu++] = gezgin;
          gezgin = onceki[gezgin];
       }
       for(int x=yol_uzunlugu-1; x>=0; x--){
          int idx = yol_indexleri[x];
          strcat(sonuc_str, k->tum_kitaplar->veri[idx]->baslik);
          if(x > 0) strcat(sonuc_str, "\n  ⬇\n");
       }
    } else {
       strcpy(sonuc_str, "Seçilen kritere göre iki kitap arasında bağlantı bulunamadı.");
    }

    while(q->on != NULL) kuyruk_cikar(q);
    free(q); free(bak); free(onceki);
    return sonuc_str;
}

DiziListe* detayli_filtrele(DiziListe* kaynak, int min_yil, int max_yil, const char* yazar_parca, const char* baslik_parca, const char* aranan_etiket, float min_pop, float max_pop){
    DiziListe* sonuc = diziliste_olustur();
    int uygun_mu, bulundu, i, j;

    char* s_yazar = (yazar_parca) ? tr_normalize(yazar_parca) : NULL;
    char* s_baslik = (baslik_parca) ? tr_normalize(baslik_parca) : NULL;
    char* s_etiket = (aranan_etiket) ? tr_normalize(aranan_etiket) : NULL;

    for(i=0; i < kaynak->adet; i++){
        uygun_mu = 1;
        Kitap* ktp = kaynak->veri[i];

        if(s_yazar && strlen(s_yazar) > 0){
            char* k_yazar = (ktp->yazar) ? tr_normalize(ktp->yazar) : NULL;
            if(k_yazar == NULL || strstr(k_yazar, s_yazar) == NULL) uygun_mu = 0;
            if(k_yazar) free(k_yazar);
        }

        if(uygun_mu == 1 && s_baslik && strlen(s_baslik) > 0){
            char* k_baslik = (ktp->baslik) ? tr_normalize(ktp->baslik) : NULL;
            if(k_baslik == NULL || strstr(k_baslik, s_baslik) == NULL) uygun_mu = 0;
            if(k_baslik) free(k_baslik);
        }

        if(uygun_mu == 1 && min_yil != -1 && ktp->yil < min_yil) uygun_mu = 0;
        if(uygun_mu == 1 && max_yil != -1 && ktp->yil > max_yil) uygun_mu = 0;

        if(uygun_mu == 1 && min_pop >= 0 && ktp->pop < min_pop) uygun_mu = 0;
        if(uygun_mu == 1 && max_pop >= 0 && ktp->pop > max_pop) uygun_mu = 0;

        if(uygun_mu == 1 && s_etiket && strlen(s_etiket) > 0){
            bulundu = 0;
            if(ktp->etiketler) {
                for(j=0; j < ktp->etiket_sayisi; j++){
                    char* k_etiket = (ktp->etiketler[j]) ? tr_normalize(ktp->etiketler[j]) : NULL;
                    if(k_etiket && strstr(k_etiket, s_etiket) != NULL){
                        bulundu = 1;
                        free(k_etiket);
                        break;
                    }
                    if(k_etiket) free(k_etiket);
                }
            }
            if(bulundu == 0) uygun_mu = 0;
        }

        if(uygun_mu == 1) diziliste_ekle(sonuc, ktp);
    }

    if(s_yazar) free(s_yazar);
    if(s_baslik) free(s_baslik);
    if(s_etiket) free(s_etiket);

    return sonuc;
}
