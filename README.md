# MCC25
MCC25
MPPT Battery charger

| Input Voltage     | 0 ~ 21 V  |
| ----------------: | :-------- |
|   Output Voltage  | 21 ~ 40V  |
| Output Power      | 200 W     |
| Efficiency        | > 90%     |


## Modelo 3D

<img style="background-color:white"  src="images/top.png" alt="3D" align="center" width="400"/>
<img style="background-color:white"  src="images/bottom.png" alt="3D" align="center" width="400"/>

## Placa Soldada

<img style="background-color:white"  src="images/Real_top.jpeg" alt="Soldered" align="center" width="400"/>
<img style="background-color:white"  src="images/Real_bottom.jpeg" alt="Soldered" align="center" width="400"/>
<img style="background-color:white"  src="images/Real_Side.jpeg" alt="Soldered" align="center" width="400"/>

# Guia de montagem da placa MCC25

A placa foi projetada de modo a facilitar a montagem da mesma, porém é necessário tomar alguns cuidados em sua montagem, pois existem algumas alterações no projeto.
## Passo a Passo

1. Realizar a troca do resistor R20 de valor 15OHMs para um resistor de 4.7OHMs.
   <img style="background-color:white"  src="images/R20.png" alt="Soldered" align="center" widt.h="200"/>
2. Isolar pino de referência do sensor de efeito HALL "U2" raspando as trilhas conectadas ao GND, em ambos os lados da placa, como demonstrado na imagem:
   <img style="background-color:white"  src="images/Real_top.jpeg" alt="Soldered" align="center" widt.h="200"/>
3. Não realizar a solda do resistor R7, ele será soldado em outro local da placa.
   <img style="background-color:white"  src="images/Real_top.jpeg" alt="Soldered" align="center" widt.h="200"/>
4. Após isolar o pino de referência do sensor de efeito HALL, realizar a solda dele normalmente. Após isso, realizar a solda de um resistor de 150OHMs entre o pino de referência e output do sensor como na imagem. (Certificar-se que não há curto)
   <img style="background-color:white"  src="images/Real_top.jpeg" alt="Soldered" align="center" widt.h="200"/>
5. Montagem do snubber verificar necessidade, mas dica de montagem, diodo TVS Schottiky 100V, capacitor de 4.7nF 100V e resistor 4.7KOhmns 3W.
   <img style="background-color:white"  src="images/Real_top.jpeg" alt="Soldered" align="center" widt.h="200"/>

## Tests

All tests are in [this](./tests/tests.md) separated document.
