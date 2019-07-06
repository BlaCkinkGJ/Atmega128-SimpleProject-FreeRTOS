# ATmega128 Simple FreeRTOS project

This repository uses the [ATmega128 embedded board](http://www.devicemart.co.kr/goods/view?no=32749&NaPm=ct%3Djxr7itzj%7Cci%3Dcheckout%7Ctr%3Dppc%7Ctrx%3D%7Chk%3D5152996465a38d046904b5ddd92334f551f6ac6e)
which is great to learn about the embedded board system.
I use the below picture's embedded board.

<img src="http://www.devicemart.co.kr/data/collect_img/kind_0/goods/large/32749.jpg" width=50%>

This project not only uses ATmega128 but also uses [FreeRTOS](https://www.freertos.org/) which is the best open source real-time operating system.
If you want to find the specific function, then I strongly recommend this repository's tags.

## LED Example(tags: LED_example)

This uses PD0 and PD1. One blinks every 500ms and the other blinks every 1s program. When you run this program. You have to add the driver folder to your project properties include setting!
