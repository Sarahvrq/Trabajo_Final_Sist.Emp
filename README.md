# Trabajo_Final_Sist.Emp

Proyecto ESP32S3 – Control Web de LEDs con PWM, Potenciómetro e Interrupciones

  Este proyecto implementa un sistema interactivo basado en ESP32-S3, capaz de controlar cuatro LEDs mediante una interfaz web accesible desde cualquier dispositivo conectado a la misma red WiFi. El sistema integra también un potenciómetro, dos botones físicos y lectura ADC continua, gestionando la lógica mediante interrupciones y un servidor HTTP ligero.

📌 Características principales:

    -Control remoto de 4 LEDs desde un navegador web.

    -Uso de PWM (LEDC) con resolución de 12 bits para controlar la luminosidad.
    
    -Lectura continua del potenciómetro mediante ADC continuo.
    
    -Dos botones físicos:
    
    -BOOT (GPIO 0) → Activa/desactiva todos los LEDs.
    
    -POT_B (GPIO 9) → Habilita o deshabilita el modo de control por potenciómetro.
    
    -Servidor web usando el puerto 80 con rutas dinámicas (/toggle/{id}).
    
    -Antirrebotes por software usando millis().
    
    -Conexión WiFi mediante WiFi.h.
    
    -Código compatible con Arduino-ESP32 v3.0.0.

⚙️ Hardware utilizado
    
    -ESP32S3 DevKit
    
    -4 LEDs con sus resistencias limitadoras
    
    -Potenciómetro (ADC en GPIO 8)
    
    -Botón BOOT (GPIO 0, interrupción)
    
    -Botón POT_B (GPIO 9, interrupción)
    
    -Cables de conexión y protoboard

📊 Resultados

  El sistema responde de forma rápida y estable tanto a los cambios de la interfaz web como a las interrupciones generadas por los botones físicos. La lectura del potenciómetro es fluida y el servidor HTTP mantiene la página sincronizada con los estados internos.
