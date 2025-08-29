# s21_decimal

s21_decimal.h shaxsiy kutubxonasini amalga oshirish.

💡 **Ushbu loyiha haqida biz bilan fikr-mulohazalaringizni baham ko’rish uchun [bu yerni bosing](https://new.oprosso.net/p/4cb31ec3f47a4596bc758ea1861fb624)**. Bu anonim bo’lib, jamoamizga ta’limni yaxshilashga yordam beradi. Loyihani tugatgandan so'ng darhol so'rovnomani to'ldirishni tavsiya qilamiz.

## Contents

1. [Chapter I](#chapter-i) \
    1.1. [Introduction](#introduction)
2. [Chapter II](#chapter-ii) \
    2.1. [Information](#information)
3. [Chapter III](#chapter-iii) \
    3.1. [Part 1](#part-1-реализация-функции-библиотеки-matrixh)  

**Chapter I**

![s21_decimal](misc/images/s21_decimal.png)

Yer sayyorasi, 90-yillar.

Jahon iqtisodiyoti jadal sur'atlar bilan o'sib bormoqda, birja yildan-yilga qayta-qayta ko'payib bormoqda, tobora ko'proq kompaniyalar IPOga chiqmoqda va ularning aksiyalari kotirovka qilina boshlanmoqda. Foydalanuvchilar soni, tranzaksiyalar soni, narx, komissiyalar, foizlar, moliyaviy texnik ko'rsatkichlarni hisoblash... Bu barcha ma'lumotlarning aniqligi muhimligiga ortiqcha baho berish qiyin, moliyaviy sektorda qo'llaniladigan hozirgi eskirgan ma'lumotlar turlari tufayli esa jiddiy muammolar yuzaga keladi.

Odatiy IEEE 754 (float) dagi hisob-kitob xatolari tufayli yiliga millionlab dollar yo'qotiladi, ular tizimdan shunchaki so'rilib, abadiy yo'qoladi.

Broker va birja o'rtasida uzatish paytida ma'lumotlarni qayta ishlash uchun foydalaniladigan FIX (Financial Information eXchange) ishlab chiqiluvchi protokoli bilan birgalikda ma'lumotlarni uzatish va saqlash uchun yana bir instrument kerak bo'ladi.

Keyingi yig’ilishda:

*— Shunday qilib, janoblar, eʼtibor qaratishingizni soʻrayman: koʻplab muvaffaqiyatli loyihalarda oʻzini koʻrsatgan bizning mutaxassislar guruhiga hukumat tomonidan kodli nom – Decimal – mutlaqo yangi maʼlumotlar turini ishlab chiqish vazifasi qoʻyilgan. Uning maqsadi bir necha o'n yillar davomida global moliyaviy operatsiyalarda xatoliklarni sezilarli darajada kamaytirish va ba'zi hollarda uni butunlay yo'q qilishdir. Kerakli hisob-kitoblarni tez va qulay bajarishga imkon beradigan barcha zarur mantiqiy va arifmetik operatsiyalarni tavsiflash kerak bo’ladi.*

*– Buni qarang-a, yaxshi buyurtma tushibdi, yana shunday mijozdan-a! Biz bu mijozni saqlab qolishimiz kerak, agar biz buni qila olsak, bu bizga kelajakda katta shartnomalar va'da qiladi!*

*– Ha, to'g'ri aytdingiz, shuning uchun biz qanday funksiyalarni amalga oshirishimiz kerakligini darhol tushunishimiz kerak... Takliflar?*

*– Yig'indi va ayirma...*

*– Ko'paytirish va bo'lish ...*

*– Fikringizga qo’shilaman, lekin bizga yanada ko'proq kerak!*

*— Qoldiq olish, taqqoslash va konvertatsiya operatsiyalari!*

*— Barcha yo'nalishlarda matematik yaxlitlash!*

*– Ha, menimcha, ro'yxat juda mos keladi, qani, ishga kirishamiz! Bizda bularning barini bajarish uchun ikki kunimiz bor, undan ko’p emas, shuning uchun, pand bermanglar!*

## Introduction

Ushbu loyihada siz C dasturlash tilida s21_decimal.h kutubxonasini amalga oshirishingiz kerak. Ushbu kutubxona til standartida bo'lmagan "decimal" turi bilan ishlash imkoniyatini qo'shishi kerak. Biroq, bu tur, masalan, suzuvchi nuqtali turlarga xos bo'lgan hisoblash xatolari qabul qilinishi mumkin bo'lmagan moliyaviy hisoblar uchun juda muhimdir. Ushbu loyiha doirasida siz moliyaviy ma'lumotlarni qayta ishlash masalalari bilan tanishasiz, har xil turdagi ma'lumotlarning ichki ko'rinishi masalalariga sho'ng'ib, tuzilmaviy yondashuvni mustahkamlaysiz.

## Chapter II

## Information

Decimal turi musbat 79,228,162,514,264,337,593,543,950,335 dan manfiy 79,228,162,514,264,337,593,543,950,335 gacha bo'lgan o'nli sonlarni ifodalaydi. Decimal qiymati asl sozlamalar bo’yicha 0 ga teng. Decimal koʻp sonli muhim butun va kasr sonlarni talab qiladigan va yaxlitlash xatosi boʻlmagan moliyaviy hisoblar uchun mos keladi. Ushbu tur yaxlitlash zaruratini bartaraf etmaydi. Aksincha, u tufayli yuzaga keladigan xatolar sonini minimallashtiradi.

Bo'lish va ko'paytirish natijasi yaxlitlash usuliga o'tkazilganda, natija aniqlikni yo'qotishdan ziyon ko’rmaydi.

Decimal son – bu belgidan, har bir raqam 0 dan 9 gacha bo'lgan sonli qiymatdan va son qiymatning butun va kasr qismlarini ajratib turuvchi o’nli nuqta o'rnini belgilovchi masshtablash koeffitsientidan iborat suzuvchi nuqtali qiymatdir.

Decimalning ikkilik ko'rinishi 1-razryadli belgi, 96-razryadli butun son va 96-razryadli butun sonni bo'lish uchun ishlatiladigan masshtablash koeffitsientidan iborat bo'lib, uning qancha qismi o'nli kasr ekanligini ko'rsatadi. Masshtablash koeffitsienti 0 dan 28 gacha bo'lgan oraliqdagi darajaga ko'tarilgan 10 raqamiga bevosita tengdir. Demak, Decimal ning ikkilik ko'rinishi ((-2^96 dan 2^96 gacha) / 10^(0 dan 28 gacha)) ko’rinishiga ega, bunda -(2^96-1) minimal qiymatga, 2^96-1 esa maksimal qiymatga teng.

Masshtablash koeffitsienti, shuningdek, Decimaldagi har qanday oxirgi nollarni saqlab qolishi mumkin. Ushbu oxirgi nollar arifmetik yoki taqqoslash operatsiyalaridagi qiymatga ta'sir qilmvaydi.

### Ikkilik ko’rinish

Decimalning ikkilik ko'rinishi 1-razryadli belgi, 96-razryadli butun son va butun sonni bo'lish uchun ishlatiladigan masshtablash koeffitsientidan iborat bo'lib, uning qancha qismi o'nli kasr ekanligini ko'rsatadi. Masshtablash koeffitsienti 0 dan 28 gacha bo'lgan oraliqdagi darajaga ko'tarilgan 10 raqamiga bevosita tengdir. 

Decimal soni 32-razryadli belgili butun sonlardan iborat to'rt elementli massiv ko’rinishida amalga oshirilishi mumkin (int bits[4] ;).

`bits[0]` `bits[1]` va `bits[2]` mos ravishda 96-razryadli butun sonning eng kichik, o'rta va yuqori 32 bitlarini o'z ichiga oladi.

`bits[3]` masshtablash koeffitsienti va belgisini o'z ichiga oladi va quyidagi qismlardan iborat:
- 0 dan 15 gacha bitlar, eng kichik so'z, ishlatilmaydi va nolga teng bo’lishi kerak.
- 16 dan 23 gacha bo'lgan bitlar butun sonni bo'lish uchun 10 darajani ko’rsatuvchi 0 dan 28 gacha darajadagi ko'rsatkichni o'z ichiga olishi kerak.
- 24 dan 30 gacha bo'lgan bitlar ishlatilmaydi va ular nolga teng bo'lishi kerak.
- Bit 31 belgini o'z ichiga oladi; 0 musbat va 1 manfiy ma'noni anglatadi.

E'tibor bering, bitli ko'rinish manfiy va musbat nollarni ajratib turadi. Ushbu qiymatlar barcha operatsiyalarda ekvivalent deb hisoblanishi mumkin.

### Misol:

```c
typedef struct 
{
    int bits[4];
} s21_decimal;
```

Arifmetik operatorlar


| Operator nomi | Operator  | Funksiya                                                                            | 
| ------ | ------ |------------------------------------------------------------------------------------|
| Qo'shish | + | int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)         |
| Ayirish | - | int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)         |
| Ko'paytirish | * | int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) | 
| Bo'lish | / | int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) |

Funksiyalar xatolik kodini qaytaradi:
- 0 – OK;
- 1 — son juda katta yoki cheksizlikka teng;
- 2 – son juda kichik yoki manfiy cheksizlikka teng;
- 3 – 0 ga bo'linish.

*Mantissaga to'g'ri kelmaydigan raqamlar haqida tafsilot:*
- *Arifmetik amallarni bajarishda mantissaga to‘g‘ri kelmaydigan raqamlarni olishda bank yaxlitlanishidan foydalaning (masalan, 79,228,162,514,264,337,593,543,950,335 — 0.6 = 79,228,162,514,264,337,593,543,950,334)*.

### Taqqoslash operatorlari

| Operator nomi | Operator  | Funksiya | 
| ------ | ------ | ------ |
| Kichik  | < | int s21_is_less(s21_decimal, s21_decimal) |
| Kichik yoki teng | <= | int s21_is_less_or_equal(s21_decimal, s21_decimal) | 
| Katta | \> |  int s21_is_greater(s21_decimal, s21_decimal) |
| atta yoki teng | \>= | int s21_is_greater_or_equal(s21_decimal, s21_decimal) | 
| Teng | == |  int s21_is_equal(s21_decimal, s21_decimal) |
| Teng emas | != |  int s21_is_not_equal(s21_decimal, s21_decimal) |

Qaytish qiymati:
-	0 – FALSE;
-	1 — TRUE.

### *O’zgartirgichlar

| O’zgartirgich | Funksiya | 
| ------ | ------ |
| int dan | int s21_from_int_to_decimal(int src, s21_decimal *dst) |
| float dan | int s21_from_float_to_decimal(float src, s21_decimal *dst) |
| int ga | int s21_from_decimal_to_int(s21_decimal src, int *dst) |
| float ga | int s21_from_decimal_to_float(s21_decimal src, float *dst) |

Qaytish qiymati – xatolik kodi:
- 0 – OK;
- 1 — konvertatsiya xatosi.

*float turi raqamini o’zgartirish bo’yicha tafsilot:*
- *Agar raqamlar juda kichik bo'lsa (0 < /x/ < 1e-28), xatolikni va 0 qiymatini qaytaring.*
- *Agar raqamlar juda katta bo'lsa (/x/ > 79,228,162,514,264,337,593,543,950,335) yoki cheksizlikka teng bo'lsa, xatolikni qaytaring.*
- *float turi bilan raqamni qayta ishlashda undagi barcha muhim o'nlik raqamlarni o’zgartiring. Agar bunday raqamlar 7 dan ortiq bo'lsa, raqamning qiymati 7 dan ko'p bo'lmagan muhim raqamga ega bo'lgan eng yaqin raqamga yaxlitlanishi kerak.*

*decimal turidagi sondan int turiga o’zgartirish haqida tafsilot:*
- *Agar decimal turidagi sonda kasr qism mavjud bo'lsa, u holda uni tashlab yuborish lozim (masalan, 0,9 0 ga o’zgartiriladi).* 

## Boshqa funksiyalar

| Tavsif | Funksiya                                                  | 
| ------ |----------------------------------------------------------|
| Ko’rsatilgan Decimal sonini manfiy cheksizlik tomon eng yaqin butun songacha yaxlitlaydi. | int s21_floor(s21_decimal value, s21_decimal *result)    |	
| Decimalni eng yaqin butun songacha yaxlitlaydi. | int s21_round(s21_decimal value, s21_decimal *result)    |
| Ko’rsatilgan Decimal sonning butun sonlarini qaytaradi; har qanday kasr raqamlari, shu jumladan oxiridagi nollar o'chiriladi. | int s21_truncate(s21_decimal value, s21_decimal *result) |
| Ko’rsatilgan Decimalni -1 ga ko'paytirish natijasini qaytaradi. | int s21_negate(s21_decimal value, s21_decimal *result)   |

Qaytish qiymati – xatolik kodi:
- 0 – OK;
- 1 — hisoblash xatosi.

## Chapter III

## Part 1. decimal.h kutubxonasi funksiyasini amalga oshirish

Yuqorida tavsiflangan kutubxona funksiyalarini amalga oshirishingiz kerak:

- Kutubxona gcc kompilyatoridan foydalangan holda C11 standartidagi C tilida ishlab chiqilishi kerak.
- Kutubxona kodi develop tarmog’idagi src papkasida joylashgan bo'lishi kerak.
- Eskirgan yoki iste’moldan chiqarilgan til konstruksiyalari va kutubxona funksiyalaridan foydalanmang. Til va ishlatiluvchi kutubxonalar bo’yicha rasmiy hujjatlardagi legacy va obsolete belgilariga e'tibor bering. POSIX.1-2017 standartiga qarab mo’ljal oling.
- Kod yozishda C++ uchun Google Style ((havola)[(https://google.github.io/styleguide/cppguide.html)]) ga amal qiling.
- Yechimni s21_decimal.a nomli statik kutubxona sifatida loyihalashtiring (s21_decimal.h sarlavha fayli bilan).
- Kutubxona tuzilmaviy dasturlash tamoyillari asosida ishlab chiqilishi kerak.
- Har bir funksiyadan oldin s21_ prefiksidan foydalaning.
- Check kutubxonasidan foydalanib, kutubxona funksiyalarining to'liq unit testlar bilan qoplamasini tayyorlang.
- Unit testlar har bir funksiyaning kamida 80% ini qamrab olishi kerak.
- Kutubxona va testlarni yig’ish uchun Makefile ni nazarda tuting (all, clean, test, s21_decimal.a, gcov_report maqsadlari bilan).
- gcov_report maqsadida html sahifasi shaklida gcov hisoboti shakllantirilishi kerak. Buning uchun unit testlar gcov bayroqlari bilan ishga tushirilishi kerak.
- decimalni amalga oshirayotganda, yuqoridagi misolda ko'rsatilganidek, bits butun sonli massiviga ega bo’lgan ikkilik ko’rinishga qarab mo’ljal oling. bits massividagi son razryadlarining o'rnini kuzating.
- _int128 turidan foydalanish taqiqlanadi.
- Keyingi nollarni qoldirish yoki olib tashlash mumkin (s21_truncate funksiyasidan tashqari).
- Aniqlanuvchi tur -79,228,162,514,264,337,593,543,950,335 dan +79,228,162,514,264,337,593,543,950,335 gacha raqamlarni qo'llab-quvvatlashi kerak.
