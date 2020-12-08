---------------------------------
Linux Shell
---------------------------------

---------------------------------------
Enes Akın		B171210397
Tahsin Nalcı		G171210082
Abdullah Akçam		G140910076
Deniz Sarıbayır		B181210079
Mehmet Akif Kiraz      	B181210037
----------------------------------------

Tasarıma genel bakış:
	
	Bash için program argc ile argümanları kontrol ediyor. Ettiğin argümanları fopen() ile açabilirse işliyor. Açamazsa hata verip kapanıyor. Aynı şekilde okuduğu satırlar içerisinde eğer "quit" komutuna rastlarsa bundan sonrasını işlemeyi bırakıyor. wait ile önceki tüm child processlerin bitmesini bekleyip, geriye "quit" komutu girildiğine dair return yapıyor. Return değerini dikkate alarak exit() ile çıkış yapıyor. Dökümanda istenildiği gibi her komut satırı çalıştırılmadan önce konsola yazdırılıyor. CTRL + D komutu, yani EOF içinse fopen() while döngüsü içinde çalıştıralarak sürekli döndürdüğü değer okunuyor. EOF olduğunda NULL döndüreceğinden döngüden çıkıp programı bitiriyor. 
	
	İnteraktif için sonsuz döngü içerisinde her satır okunuyor. fopen() ın döndürdüğü değer CTRL + D için kontrol ediliyor. Sorun yoksa ilk önce strtok() yardımıyla satırdaki komutların ile birbirinden ayrılmasını sağlanıyor. Ayrılan değerlerin komut olup olmadığı yine strtok() yardımıyla boşluklardan arındırılmasıyla anlaşılıyor. Komutsa two dimensional array içerisine atılıyor. Bu arrayin ilki komutları ikincisi de komutların argumanlarını tutuyor. Döküman içerisinde maksimum komut ve argüman sayısı belirtilmediğinden ilk önce büyük bir boş array oluşturuluyor. Program çalışırken performansa etkisi olmaması için daha sonra bu array komut sayısına göre hazırlanmış başka bir arraye atılıp free() ile heapten temizleniyor. Bu döndürülen array komutların çalıştırılmasını sağlayan farklı bir fonksiyona argüman olarak veriliyor. Bu fonksiyon içerisinde komut sayısı kadar fork() ile child process oluşturularak bunun içinde execvp() yardımıyla komutların çalıştırılması sağlanıyor. execvp() nin hata kontrolü sağlanıyor. Bu sayede anlamsız komutların çalışmaması durumunda, neden çalışmadığına dair konsol bilgi yazdırılıyor. Ayrıca her döngü sonunda heapte oluşturulmuş tüm değerlerin temizlenerek memory leak'e neden olması engelleniyor.
	
Özel durumlar:

	Yazılım hiçbir hata karşısında çökmeyecek şekilde tasarlandı. Girilen komutlarda whitespace, fazladan ";", ";" arasında komut olmaması gibi hiçbir durum programın düzgün çalışmasını engellemiyor. Program her zaman düzgün bulduğu komutları çalıştırıyor. Anlamsız bir komut girilmişse ve bu komut çalıştırılamadıysa hatası konsola döndürülüyor. Tüm bunlar strtok() ile önce ";" sonra boşluklardan arındırılarak sağlandı.
	
	Program çalıştırılırken yanlış komut sayıda girilmesi, istenilen dosyanın bulunmaması, CTRL + D basılması durumda EOF'nin değerlendirilmesi gibi tüm olasılıklar, fonksiyonların geri dönüş değerlerinin okutulması ve buna göre çalıştırılmasıyla hesaplandı.
	
Bilinen hatalar ve sorunlar:
	
	Test ettiğimiz kadarıyla hiçbir soruna rastlamadık.


