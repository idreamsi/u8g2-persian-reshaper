## 📖 About
Reconstruct Persian/Arabic sentences for use in the [u8g2 library](https://github.com/olikraus/u8g2/). In languages like Persian/Arabic, the letter is modified depending on where it should go in a word. Usually, when you print to the display ordinary Persian/Arabic text, each character is printed individually and is not joined together, whereas a computer would join it automatically on the screen but not modifying the actual data of each character.

## 📚 Description
Persian/Arabic script is very special with two essential features:
- It is written from right to left.
- The characters change shape according to their surrounding characters.

So when you try to print text written in Persian/Arabic script on LCD/OLED display that doesn’t support Persian/Arabic you’re pretty likely to end up with something that looks like this:

<p align="center">
  <img src="http://mpcabd.xyz/wp-content/uploads/2012/05/arabic-1.png">
</p>

## 📷 Screenshot
<p align="center">
  <img width="516" height="600" src="https://github.com/idreamsi/u8g2-persian-reshaper/blob/master/screenshot.jpg?raw=true">
</p>

## ⚙️ Download & Usage
Download/Clone repository, then Install [u8g2 library](https://github.com/olikraus/u8g2/wiki/u8g2install). Change [this line](https://github.com/idreamsi/u8g2-persian-reshaper/blob/629c98f3ab882f6cb7662f8ce0c2ef2c2fa8111d/u8g2-persian-reshaper.ino#L15) depending on the display type and wiring. Finally upload the code.

[راهنمای استفاده به زبان فارسی](http://www.idreams.ir/%d9%81%d8%a7%d8%b1%d8%b3%db%8c-%d9%86%d9%88%db%8c%d8%b3%db%8c-%d8%af%d8%b1-%d8%a2%d8%b1%d8%af%d9%88%db%8c%d9%86%d9%88-%d8%a8%d8%a7-%da%a9%d8%aa%d8%a7%d8%a8%d8%ae%d8%a7%d9%86%d9%87-u8g2/)

[راهنمای ساخت فونت جدید](http://www.idreams.ir/%da%86%da%af%d9%88%d9%86%da%af%db%8c-%d8%b3%d8%a7%d8%ae%d8%aa-%d9%81%d9%88%d9%86%d8%aa-%d8%af%d9%84%d8%ae%d9%88%d8%a7%d9%87-%d8%a8%d8%b1%d8%a7%db%8c-%da%a9%d8%aa%d8%a7%d8%a8%d8%ae%d8%a7%d9%86%d9%87-u8/)

## 📫 Contact
Copyright (C) 2019 [Ramin Sangesari](mailto:r.sangsari@gmail.com).

## 💵 Donation
If this project help you reduce time to develop, you can give me a cup of coffee :)

- Donate via Bitcoin: 176H7tbpfyUVKiwkkwj6BJfC1X7hQWTq8v
- [Donate via IDPay](https://idpay.ir/idreams)
