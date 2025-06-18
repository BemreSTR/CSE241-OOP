# Robot Savaş Simülasyonu Ödevi İçin Yol Haritası

## 1. Proje Yapısını Oluşturma
- **Dosya yapısını oluştur**: Robot.h, Robot.cpp, World.h, World.cpp ve main.cpp dosyalarını oluştur
- **Basit bir Makefile** hazırla
- **Git** gibi bir versiyon kontrol sistemi kullanmaya başla

## 2. Sınıf Hiyerarşisini Tasarlama
- **Robot** temel sınıfını oluştur
- İlk seviye türetilmiş sınıflar: **Humanic**, **Roomba**, **Bulldozer**, **Kamikaze**
- İkinci seviye türetilmiş sınıflar: **OptimusPrime**, **Robocop** (Humanic'ten türetilecek)

## 3. Temel Sınıfları Kodlama
- **Robot** sınıfını kodla:
  - Gerekli özellikler: strength, hitpoints, name
  - Sanal fonksiyonlar: getType(), getDamage(), move(), isAlive()
  - Constructor ve destructor yapılandırması

- **World** sınıfını kodla:
  - 10x10 ızgara oluştur (Robot işaretçileri için 2B dizi)
  - Robotları ızgaraya yerleştirme işlevleri
  - Hareket ve savaş mekanizmaları için fonksiyonlar

## 4. Türetilmiş Robot Sınıflarını Kodlama
- Her sınıf için getDamage() fonksiyonunu özel davranışlara göre uygula:
  - **Humanic**: %15 ihtimalle ekstra 40 hasar
  - **OptimusPrime**: %20 ihtimalle çift hasar
  - **Roomba**: İki kez saldırı
  - **Kamikaze**: Hitpoints kadar hasar, sonra ölüm
  - **Bulldozer**: Temel davranış

- Her sınıf için getType() fonksiyonunu uygula

## 5. Simülasyon Mantığını Kodlama
- **SimulateOneStep()** fonksiyonunu uygula:
  - Robotların hareket ettirilmesi
  - Robotlar arası çatışmaların gerçekleştirilmesi
  - Humanic robotlar için iyileşme mekanizması

- **Çatışma mekanizması** için bir fonksiyon yaz:
  - İki robot arasındaki savaş döngüsü
  - Hasar hesaplama ve hitpoint güncelleme
  - Hit mesajlarını formata uygun yazdırma

## 6. main.cpp Dosyasını Yazma
- Rastgele sayı üretecini başlat
- World nesnesini oluştur
- Her robot tipinden 5'er tane oluştur ve ızgaraya yerleştir
- Simülasyon döngüsünü başlat

## 7. Test ve Hata Ayıklama
- **Bellek sızıntılarını kontrol et**: Dinamik bellek tahsislerinin düzgün şekilde serbest bırakıldığından emin ol
- **Davranış testleri**: Her robot tipinin beklenen davranışı sergilediğinden emin ol
- **Sınır durumlarını test et**: Izgara kenarlarındaki hareketler, tek robot kalana kadar simülasyon vb.

## 8. Optimizasyon ve Son Rötuşlar
- Kodun okunabilirliğini artır: düzgün girintileme ve anlamlı yorumlar ekle
- Gereksiz kod tekrarlarını temizle
- Çıktı formatının ödevde belirtildiği gibi olduğundan emin ol

## 9. Derleme ve Paketleme
- Kodun Linux ortamında (g++ ile) hatasız derlenebildiğini kontrol et
- Tüm kaynak dosyalarını içeren bir ZIP dosyası oluştur
- Dosya adlandırma kurallarına uygun şekilde gönderime hazırla

Her adımda düzenli test yapman ve işlevleri birer birer eklediğinden emin olman hatalarla başa çıkmanı kolaylaştıracaktır. İyi çalışmalar!