
## Setup ambiente ESPHome e flash di un dispositivo

### 1. Wifi

Copia il file di esempio:
```
cp devices/secrets.yaml.EXAMPLE devices/secrets.yaml
```
Apri devices/secrets.yaml e inserisci i parametri della tua connessione wifi 


### 2. Flash del device

Requisiti: pipenv

```
> pipenv install

# verifica
> pipenv run esphome version
Version: 2025.4.2

# flash di un device
> pipenv run esphome run devices/scale.yaml
# e scegliere la porta alla quale è connesso l'ESP32

# oppure (se già flashato), logs via seriale
> pipenv run esphome logs devices/scale.yaml
# e scegliere la porta alla quale è connesso l'ESP32
```


