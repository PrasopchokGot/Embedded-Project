# Embedded-Project Group 11 Application
(Version 1.0 28/04/2023)
## Objective
ในปัจจุบันสังคมในช่วงวัยของผู้สูงอายุมีแนวโน้มเพิ่มขึ้น อีกทั้งยังมีช่วงวัยเด็กในวัยกำลังพัฒนาที่จะเจริญเติบโตขึ้นไปอีก การพัฒนาทักษะต่าง ๆ จึงเป็นเรื่องที่สำคัญยิ่ง สำหรับในวัยผู้สูงอายุนั้นปัญหาที่อาจพบเจอได้มากคืออาการอัลไซเมอร์หรือกล้ามเนื้อมืออ่อนแรงและสำหรับในวัยเด็กการพัฒนากล้ามเนื้อมือและสมองก็สำคัญไม่แพ้กัน อีกทั้งยังมีการพัฒนาทางด้านความจำ ความคิด และความว่องไวอีกด้วย ดังนั้นการมีเครื่องมือที่ช่วยพัฒนาให้เกิดการพัฒนาทักษะเหล่านี้ได้จะทำให้เกิดผลดีต่อสุขภาพซึ่งยังให้เกิดผลดีต่อเศรษฐกิจและสังคมโดยรวม ประชาชนมีสุขภาพดี ค่าใช้จ่ายด้านสุขภาพน้อยลง โดยอุปกรณ์ที่คิดค้นขึ้นมาจะเป็นเกมสำหรับพัฒนาทักษะเหล่านี้ซึ่งจะมีทั้งหมด 2 เกม เกมแรกจะเป็นการพัฒนาทักษะทางด้านกล้ามเนื้อมือ ความคิดและความว่องไว และเกมที่สองจะเป็นการพัฒนาทักษะในด้านกล้ามเนื้อมือและความจำ ซึ่งทุกวัยสามารถเล่นได้เช่นกันไม่ว่าจะเป็นเพื่อการพัฒนาทักษะ เพื่อความเพลิดเพลินหรือเพื่อการลดความเครียดให้มีสุขภาพจิตที่ดี

สำหรับอินพุตของระบบจะมีการใช้ IR Module เพื่อเป็นการเลือกเกม
ในเกมแรกจะมีอินพุตเป็น Matrix Keypad Module สำหรับกด และเอาท์พุตคือ 8×8 LED DotMatrix จะออกมาเป็นจุด ๆ เพื่อให้กด Matrix Keypad Module ตาม สำหรับเกมสองจะมีอินพุตเป็น Analog Joystick Module สำหรับเลื่อนเคลื่อนไหวและเอาท์พุตคือ 8×8 LED DotMatrix จะออกมาเป็นเส้นทาง

## Hardware
- Arduino Mega 2560
- Breadboard 
- RGB LED
- IR Receiver
- 4 digit 7 segment 
- 8×8 LED DotMatrix
- I2C 16x2 LCD Display 
- Matrix Keypad Module
- Analog Joystick Module
- RTC DS3231
- IR Module
- Buzzer 
- Resistance 220 Ohm
- Resistance 1000 Ohm

## Hardware configuration
การต่อวงจรสามารถทำได้ตาม schematic diagram ด้านล่าง

![B0EAE56F-4B00-485C-AD58-94C9EA277C82](https://user-images.githubusercontent.com/68359540/235310753-38cb880b-b8d1-467f-acf5-3990ae648fbb.jpg)

## Flow Chart
เกมจะมีหลักการทำงานเป็นตาม Flow Chart ด้านล่าง

![Untitled Diagram drawio (2)](https://user-images.githubusercontent.com/68359540/235311576-93941b4a-c993-4ee6-ad0e-125b3c3ea98d.jpg)

## How to Play
1. ใช้ IR Module เพื่อเลือกเกมที่จะเล่นโดยหมายเลข 1 คือ เกมพัฒนาความคิดและพัฒนากล้ามเนื้อ และหมายเลขที่ 2 คือ เกมฝึกความจำ และปุ่ม CH- คือ ปุ่มออกจากเกม
2. **เกมพัฒนาความคิดและพัฒนากล้ามเนื้อ** ในเกมนี้จะมีสี่เหลี่ยม 2x2 ขึ้นบนจอ 8x8 Dot Matrix เป็นเวลา 1 วินาที ให้ผู้เล่นใช้ Matrix Keypad Module ในการกดเล่นเกม โดยจะต้องกดในตำแหน่งเดียวกันกับที่สี่เหลี่ยม 2x2 แสดงขึ้นมาบนจอ 8x8 Dot Matrix แสดงขึ้นมาจึงจะได้คะแนน 1 แต้ม หลังจากนั้นจะมีสี่เหลี่ยม 2x2 แสดงขึ้นมาใหม่เรื่อย ซึ่งอาจจะเป็นตำแหน่งเดิมหรือตำแหน่งใหม่ก็ได้ วนซ้ำแบบนี้ไปเรื่อยจนกว่าจะกดผิดตำแหน่ง คะแนนที่ได้จะถูกแสดงบนจอ 4 digit senven segment
4. **เกมฝึกความจำ** เส้นทางแบบสุ่มจะถูกสุ่มขึ้นบนหน้าจอ 8x8 Dot Matrix เป็นเวลา 5 วินาที หลังจากนั้นจะดับลงไป ผู้เล่นจะมีตำแหน่งเริ่มต้นอยู่ที่จุดเริ่มต้นของเส้นทาง ผู้เล่นจำเป็นต้องเดินไปตามที่จำได้เมื่อ 5 วินาทีทีแล้วไปจนถึงปลายทางออก เมื่อเคลื่อนที่ไปถึงได้โดยไม่ออกจากเส้นทางที่ถูกต้องเลยจะสามารถชนะเกมได้ ระหว่างเล่นเกมจะมีการจำเวลาดำเนินไปเรื่อยๆ **หากชนะ** ไฟสีเขียวจะติด และมีเวลาที่ใช้ในการเดินทางแสดงอยู่บนจอ 4 digit seven segment **หากแพ้** ไฟสีแดงจะติด และมีเสียง Beep ดังขึ้น และไม่แสดงเวลาที่ใช้
