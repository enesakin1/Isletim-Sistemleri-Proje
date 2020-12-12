## Linux Shell

| Ogrenci        | Numara       |
| ------------- |:-------------:|
| Enes Akın      | B171210397 |
| Tahsin Nalcı		|	G171210082|
| Abdullah Akçam	|		G140910076|
| Deniz Sarıbayır	|		B181210079|
| Mehmet Akif Kiraz     | 	B181210037|

### Tasarıma genel bakış
	
Bash için program "argc" ile argümanları kontrol ediyor. Kontrol ettiği argümandaki dosyayı "fopen()" ile açabilirse işliyor. Açamazsa hata verip kapanıyor. Aynı şekilde okuduğu satırlar içerisinde eğer "quit" komutuna rastlarsa bundan sonrasını işlemeyi bırakıyor. "wait()" ile önceki tüm child processlerin bitmesini bekleyip, geriye "quit" komutu girildiğine dair değer döndürüyor. Return değerini dikkate alarak "exit()" komutuyla programdan çıkılıyor. Dökümanda istenildiği gibi, her komut satırı çalıştırılmadan önce konsola yazdırılıyor. "CTRL + D" komutu, yani "EOF" için, "fopen()" while döngüsü içinde çalıştıralarak, sürekli döndürdüğü değer okunuyor. "EOF" olduğunda "NULL" döndüreceğinden, döngüden çıkıp programı bitiriyor. 

İnteraktif için sonsuz döngü içerisinde her satır okunuyor. "fopen()"ın döndürdüğü değer "CTRL + D" için kontrol ediliyor. Sorun yoksa ilk önce "strtok()" yardımıyla satırdaki komutların birbirinden ayrılmasını sağlanıyor. Ayrılan değerlerin komut olup olmadığı yine "strtok()" yardımıyla, boşluklardan arındırılmasıyla anlaşılıyor. Komutlar da two dimensional array içerisine atılıyor. Bu arrayin ilki komut sırasını, ikincisi de komutların argumanlarını tutuyor. Döküman içerisinde maksimum komut ve argüman sayısı belirtilmediğinden, ilk önce büyük bir boş array oluşturuluyor. Program çalışırken performansa etkisi olmaması için daha sonra bu array komut sayısına göre hazırlanmış başka bir arraye atılıp "free()" ile heapten temizleniyor. Bu döndürülen array, komutların çalıştırılmasını sağlayan farklı bir fonksiyona argüman olarak gönderiliyor. Bu fonksiyon içerisinde komut sayısı kadar "fork()" ile child process oluşturularak, "execvp()" yardımıyla komutların çalıştırılması sağlanıyor. "execvp()"nin hata kontrolü sağlanıyor. Bu sayede anlamsız komutların çalışmaması durumunda, neden çalışmadığına dair konsola bilgi yazdırılıyor. Ayrıca her döngü sonunda heapte oluşturulmuş tüm değerlerin temizlenerek, yazılım sonucunda memory leak oluşması engelleniyor.
	
## Özel durumlar:

Yazılım hiçbir hata karşısında çökmeyecek şekilde, istenildiği gibi tasarlandı. Girilen komutlarda whitespace, fazladan ";", ";" arasında komut olmaması gibi hiçbir durum programın düzgün çalışmasını engellemiyor. Program her zaman düzgün okuduğu komutları çalıştırıyor. Anlamsız bir komut girilmişse ve bu komut çalıştırılamadıysa hatası konsola döndürülüyor. Tüm bunlar strtok() ile önce ";" ile komuların belirlenmesiyle, sonrasında da boşluklardan arındırılmasıyla sağlandı.

Program çalıştırılırken fazla argüman girilmesi, istenilen dosyanın bulunmaması, "CTRL + D"ye basılması durumda "EOF"nin değerlendirilmesi gibi tüm olasılıklar, fonksiyonların geri dönüş değerlerinin okutulması ve buna göre çalıştırılmasıyla hesaplandı.
	
## Bilinen hatalar ve sorunlar
	
Test ettiğimiz kadarıyla hiçbir soruna rastlamadık.


