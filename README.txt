#Mert Can Gönen
#181101039

-Çalıştırma Aşamaları-
1) gcc DiningPhilosophers.c -o df -lpthread
2) ./df [x]

Hocam kod üzerinde neredeyse her satırda detaylı açıklamalar var. Filozof threadlerini oluştururken segmentation fault aldım. 
Malumunuz sınav haftamız olduğu için bu soruna çok yoğunlaşamamıştım.
Belki bugün(09-07-2022) bir çözüm bulabilirim diye geç gönderdim ancak bir türlü sorunu çözemedim.

Koda gelirsek; 
--Waiter fonksiyonu waiter thread için oluşturuldu. Gün 1000'i geçmediği sürece servis yapıyor ve günü arttırıp 10ms uyuyor. 1000 gün sonunda exit işlemi ile thread çıkıyor.
--Serve fonksiyonunda eğer masada pirinç varsa waiter bekliyor. Pirinç bittikten sonra o günün servisini yapıyor. O gün gelip baktığında hala pirinç varsa o günü boş geçmiyor. 
  Ödev metninden böyle bir çıkarımda bulundum. Ancak bunu yapmak istemeseydik 44.satırda sleep yerine return; diyip o günü servis yapmadan geçirebilirdik.
  Eğer gün 1'den büyükse random sayıda pirinç oluşturup servisi yapıyor. Eating işlemlerinde kimi ne kadar yediği gerekli array'lere kaydedildiği için sadece aç gün hesaplamalarını burada yaptım. 
  Yemediyse aç güün 1 arttır, yediyse son aç gün streak'i ile max aç günü streak'ini karşılaştırıp ona göre seçiyorum.
--Eat fonksiyonunda stick'leri alan filozof pirinç yoksa 2 saniye bekliyor. Belirli bir sayı verilmemiş ben de hem thinking hem de bekleme durumunu 2 saniye yaptım. 
  Pirinç varsa günlük pirinci 1 azaltıp kendi hanelerine ekliyor.
--Take fonksiyonunda çubukları kontrol edip yemek işlemini gerçekleştiriyor. Check adlı binary semaphore'un tutulma sebebini kod üzerinde detaylı şekilde açıkladım. 
  Yedikten sonra 2 saniye düşündüğünü varsaydım az önce de bahsettiğim gibi.
--Philosopher fonksiyonu filozof thread'leri için oluşturuldu. Gelen filozofun numarasıyla beraber eğer restoran açıksa take işlemleri başlıyor.
--Main içerisinde önce filozof sayısına göre tutmak istediğim array'lerin alanlarını oluşturdum. Ardından waiter ve philosopher thread'leri tanımladım. Initial değerleri set ettim. 
  İlk başta 0 yemek yendi, çubukların semaphore'ları 1 vs. Ardından waiter ve philosopher thread'leri oluşturdum (filozof threadleri segmentation fault attı maalesef). Join işlemini yaptım. 
  Gerekli outputları bastırdım. Sonra ilk filozof'a daha yüksek priority atadım ve aynı initial işlemleri tekrar gerçekleştirdim. Son çıktıları da bastırıp programdan çıktım.    

NOT => Ödev metninde herhangi bir şekilde eat-think-sleep döngüsünden bahsedilmediği için bunları tanımlayan bir array ya da veri yapısı tanımlamadım. Yemek isteyen filozoflar sağı ve solundaki filozofların state'i yerine orada bulunan stick'leri kontrol ederek yemek yiyiyor.
       Zaten stick boştaysa yandaki filozof ya think ya da sleep işlemindedir.