# MCC25
MCC25  
MPPT Battery charger  

| Input Voltage     | 0 ~ 21 V  |
| ----------------: | :-------- |
|   Output Voltage  | 21 ~ 40V  |
| Output Power      | 200 W     |
| Efficiency        | > 90%     |

## Modelo 3D

<p align="center">
  <img src="images/top.png" alt="3D Top" width="44.5%"/>
  <img src="images/bottom.png" alt="3D Bottom" width="41.23%"/>
</p>

## Placa Soldada

<p align="center">
  <img src="images/Real_top.jpeg" alt="Soldered Top" width="30%"/>
  <img src="images/Real_bottom.jpeg" alt="Soldered Bottom" width="30%"/>
  <img src="images/Real_Side.jpeg" alt="Soldered Side" width="19.7%"/>
</p>

# Guia de montagem da placa MCC25

A placa foi projetada para facilitar a montagem, mas é necessário tomar alguns cuidados devido a algumas alterações no projeto.

## Passo a Passo

| Passo | Instruções | Imagem |
|------|------------|--------|
| **1** | Realizar a troca do resistor R20 de 15Ω para um resistor de 4.7Ω. | <img src="images/R20.png" alt="Montagem" width="200"/> |
| **2** | Isolar o pino de referência do sensor de efeito HALL "U2" raspando as trilhas conectadas ao GND, em ambos os lados da placa. | <img src="images/Hall_reference.jpeg" alt="Montagem" width="200"/> |
| **3** | Não realizar a solda do resistor R7, pois ele será soldado em outro local da placa. | <img src="images/R7.png" alt="Montagem" width="200"/> |
| **4** | Após isolar o pino de referência do sensor de efeito HALL, realizar a solda dele normalmente. Em seguida, soldar um resistor de 150Ω entre o pino de referência e o output do sensor. (Certificar-se de que não há curto-circuito). | <img src="images/RES_HALL.jpeg" alt="Montagem" width="200"/> |
| **5** | Montagem do snubber: verificar a necessidade, mas a dica de montagem é usar um diodo TVS Schottky 100V, capacitor de 4.7nF 100V e resistor 4.7KΩ 3W. | <img src="images/Snubber.jpeg" alt="Montagem" width="200"/> |

## Testes

Em processo...
