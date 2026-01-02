# MÜHENDİSLİK VE DOĞA BİLİMLERİ FAKÜLTESİ  
# BİLGİSAYAR MÜHENDİSLİĞİ BÖLÜMÜ  
# VERİ YAPILARI DERSİ  
# FİNAL PROJESİ


İSTANBUL MEDENİYET ÜNİVERSİTESİ  
MÜHENDİSLİK VE DOĞA BİLİMLERİ FAKÜLTESİ  

BİLGİSAYAR MÜHENDİSLİĞİ BÖLÜMÜ  
VERİ YAPILARI DERSİ FİNAL PROJESİ  

**Proje Adı:** Gelişmiş Veri Yapıları ile Kütüphane Yönetim Sistemi  

**Hazırlayanlar:**  

24120205040 - Nisa Gökşen  
24120205009 - İsmail Taner Erdoğan  

**Ders Sorumlusu:** Dr. Öğr. Üyesi Muhammet Sinan BAŞARSLAN  

**Tarih:** Ocak, 2026  

**Yer:** İstanbul  


---

## 1. GİRİŞ

Günümüzde bilgiye erişim hızı ve verilerin doğru yönetimi, yazılım  
sistemlerinin en kritik gereksinimlerinden biridir. Kütüphane sistemleri gibi  
büyük veri setlerinin bulunduğu ortamlarda, verilerin sadece saklanması  
değil; hızlı aranması, sıralanması ve ilişkisel bağların kurulması  
gerekmektedir. Standart veri saklama yöntemleri (basit diziler veya listeler),  
veri miktarı arttıkça performans sorunlarına yol açmakta ve kullanıcı  
deneyimini olumsuz etkilemektedir.

Bu projenin temel amacı, Veri Yapıları dersi kapsamında öğrenilen teorik  
bilgilerin gerçek hayat senaryosuna uygulanmasıdır. Proje kapsamında,  
kitapların eklenmesi, silinmesi, güncellenmesi ve aranması gibi temel  
işlemlerin yanı sıra; kitaplar arası ilişkilerin (BFS algoritması ile) bulunması,  
hatalı girişlerin geri alınması (Undo/Redo) ve akıllı arama (Trie/TF-IDF) gibi  
ileri seviye özellikler geliştirilmiştir.

Projede veri tutarlılığını sağlamak ve erişim sürelerini minimize etmek  
amacıyla AVL Ağacı (Dengeli Arama Ağacı), Trie (Prefix Ağacı), Yığıt  
(Stack) ve Kuyruk (Queue) gibi veri yapıları hibrit bir mimaride kullanılmıştır.  
Bu sayede, milyonlarca kayıt olsa dahi işlemlerin logaritmik veya sabit  
karmaşıklıkta (Complexity) gerçekleştirilmesi hedeflenmiştir.


---

## 2. MATERYAL VE METOD

Bu bölümde, projenin geliştirilmesinde kullanılan yazılım dilleri, geliştirme  
ortamları (IDE) ve en önemlisi sistemin omurgasını oluşturan veri yapıları  
detaylandırılmıştır.

### 2.1. Kullanılan Teknolojiler ve IDE'ler

Projenin "Backend" (Arka Plan) mantığı, bellek yönetimi ve pointer aritmetiği  
üzerindeki hakimiyeti göstermek amacıyla saf C dili ile geliştirilmiştir.  
Kullanıcı Arayüzü (GUI) tarafında ise nesne yönelimli yapısı ve güçlü  
kütüphane desteği nedeniyle C++ ve Qt Framework tercih edilmiştir.

**IDE:** Qt Creator / Visual Studio Code  
**Dil:** C (Veri Yapıları Kütüphanesi) ve C++ (Arayüz Bağlantısı)  
**Veri Formatı:** CSV (Comma-Separated Values) - Verilerin kalıcı olarak  
saklanması için.

### 2.2. Kullanılan Veri Yapıları

Projede tek bir veri yapısına bağlı kalınmamış, her problemin doğasına en  
uygun yapı seçilmiştir:

#### 2.2.1. AVL Ağacı (AVL Tree)

Kitapların benzersiz ID numaralarına göre saklanması, silinmesi ve  
güncellenmesi işlemleri için kullanılmıştır. Standart Binary Search Tree (BST)  
aksine, AVL ağacı her ekleme/silme işleminden sonra kendini dengeleyerek  
ağaç yüksekliğini her zaman log(n) seviyesinde tutar. Bu sayede "Worst-  
Case" senaryosunda bile arama performansı korunmuştur.

#### 2.2.2. Trie (Prefix Tree)

Kullanıcı arama çubuğuna bir kitap isminin baş harflerini yazdığında (Örn:  
"Suç ve..."), anlık sonuç getirmek (Autocomplete) için kullanılmıştır. Kelime  
uzunluğuna bağlı (O(k)) arama hızı sunduğu için veri seti ne kadar büyürse  
büyüsün performansı düşmez.

#### 2.2.3. Yığıt (Stack)

Kullanıcı deneyimini artırmak için "Geri Al" (Undo) ve "İleri Al" (Redo)  
mekanizmaları geliştirilmiştir. Yapılan her işlem (Ekleme, Silme, Güncelleme)  
bir Islem struct'ı olarak Stack veri yapısına atılır. LIFO (Last In First Out)  
prensibi sayesinde işlemlerin geri alınması sağlanır.

#### 2.2.4. Kuyruk (Queue) ve Graf (Graph)

Kitaplar arasındaki ilişkileri (Aynı yazar, aynı yıl, benzer puan vb.) analiz  
etmek için Graf yapısı kullanılmıştır. İki kitap arasındaki en kısa ilişki yolunu  
bulmak için ise BFS (Breadth-First Search) algoritması kullanılmış ve gezinti  
sırasında düğümler Kuyruk yapısında tutulmuştur.


---

## 3. UYGULAMA

Bu bölümde, tasarlanan sistemin mimarisi, arayüz tasarımı ve algoritmaların  
uygulama detayları açıklanmıştır.

### 3.1. Tasarım ve Arayüz

Uygulama arayüzü, kullanıcı dostu (User Friendly) olması açısından modern  
Qt bileşenleri ile tasarlanmıştır.

**Dinamik Liste:** Kitaplar "Kart (Card)" yapısında listelenir.  

Şekil 1: Kütüphane Yönetim Sistemi Ana Arayüzü  

**Filtreleme Paneli:** Kullanıcıların Yıl, Puan, Yazar ve Kategoriye göre  
detaylı filtreleme yapabildiği bir "Overlay" panel tasarlanmıştır.

Şekil 2: Filtreleme Modülü  

**Görselleştirme:** BFS sonuçları metin yerine, kitaplar arası bağlantıyı  
gösteren ok işaretleri (⬇) ile görselleştirilmiştir.

Şekil 3: BFS Sonuç Ekranı  

### 3.2. Gerçeklenen Senaryolar ve Algoritmalar

1. Türkçe Karakter Normalizasyonu: C dilinde char tabanlı işlemlerde  
yaşanan Türkçe karakter (İ, ı, ş, ğ) sorunları, özel olarak yazılan  
tr_normalize fonksiyonu ile çözülmüştür. Bu fonksiyon, byte seviyesinde  
kontrol yaparak sıralama ve arama işlemlerinin hatasız çalışmasını sağlar.

2. Akıllı Sıralama (Smart Sort): Kullanıcı "Sıralama" seçeneğini  
kullandığında Merge Sort ve Quick Sort algoritmaları devreye girer.  
Merge Sort, kararlı (stable) yapısı nedeniyle tercih edilmiştir.

3. Arama Motoru (TF-IDF & Trie): Kullanıcı tam ismi hatırlamasa bile, içerik  
içinde geçen kelimelere göre alaka düzeyini hesaplayan basitleştirilmiş bir  
TF-IDF algoritması ve Trie yapısı hibrit olarak çalışmaktadır.


---

## 4. SONUÇ VE TARTIŞMA

Yapılan testler sonucunda, uygulamanın büyük veri setlerinde dahi yüksek  
performansla çalıştığı gözlemlenmiştir. Özellikle AVL ağacının kullanımı  
sayesinde, 10.000+ kitaplık veri setlerinde dahi ID ile arama işlemi  
milisaniyeler mertebesinde gerçekleşmiştir1111.

**Karşılaşılan Zorluklar ve Çözümler:**

**Zorluk:** C ve C++ dilleri arasında veri taşırken yaşanan bellek yönetimi  
(Memory Leak) riskleri.  
**Çözüm:** Her malloc işlemi için karşılık gelen free işlemleri titizlikle yazılmış  
ve Destructor metodları ile bellek temizliği garanti altına alınmıştır.

**Zorluk:** Türkçe karakterlerin (UTF-8) sıralamada (Sorting) ASCII  
değerlerine göre yanlış yerleşmesi.  
**Çözüm:** Özel bir karşılaştırma (Comparator) fonksiyonu yazılarak Türkçe  
karakterlerin alfabedeki doğru yerlerine göre sıralanması sağlanmıştır.

Proje, sadece bir veri tabanı uygulaması olmanın ötesinde, veri yapılarının  
(Tree, Graph, Stack) gerçek dünyadaki problemlerin çözümünde ne kadar  
etkili olduğunu somut bir şekilde ortaya koymuştur. Gelecek çalışmalarda  
sistemin bir SQL sunucusuna bağlanması ve Web arayüzüne taşınması  
hedeflenebilir.


---

## 5. KAYNAKÇA

1. Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009).  
Introduction to Algorithms. MIT press.

2. Sedgewick, R., & Wayne, K. (2011). Algorithms. Addison-Wesley  
Professional.

3. Knuth, D. E. (1997). The Art of Computer Programming. Addison-Wesley.

4. Qt Documentation. (2025). "Qt Framework and C++ Integration".

5. Chat GPT ve Gemini’den yardım alındı.


---

## 6. EKLER

Proje Kaynak Kodları (GitHub): [Buraya GitHub Linkini Yapıştır]  
Uygulama Tanıtım Videosu: [Varsa Link]
