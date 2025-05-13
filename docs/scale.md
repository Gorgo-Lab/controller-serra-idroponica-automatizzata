# Scale

## Celle di carico
Una cella di carico di solito ha 4 pin.
Noi usiamo 4 "mezze celle di carico", a 3 pin ognuna, pensate per essere usate in parallelo, non da sole.

La cella di carico sfrutta la deformazione elastica di un materiale usando degli estensimetri:

1. Quando si applica un peso sulla cella, il metallo si deforma leggermente.
2. Gli estensimetri (fili sottili resistivi) incollati sul metallo cambiano la loro resistenza in proporzione alla deformazione.
3. Questi estensimetri sono collegati in una configurazione chiamata ponte di Wheatstone, che serve per:
    - rilevare variazioni piccolissime di resistenza,
    - convertire la variazione in un segnale di tensione differenziale molto piccolo (tipicamente in millivolt).


Quando si collegano le 4 celle secondo lo schema, i loro fili si combinano in modo da:
- Bilanciare l'uscita in assenza di peso
- Produrre una differenza di tensione tra due pin (S+ e S−) proporzionale al peso applicato


Questa configurazione si chiama *ponte di Wheatstone*.
Vantaggi del ponte di Wheatstone:


- Massima sensibilità: somma gli effetti di tutte e 4 le celle
- Stabilità e precisione: riduce il rumore, compensa eventuali piccoli errori o disallineamenti
- Peso distribuito: puoi appoggiare il carico ovunque sulla bilancia, e verrà comunque misurato correttamente


Dopo il collegamento, si ottengono 4 fili / PIN finali (di solito etichettati così):

- E+ → Alimentazione positiva
- E− → GND
- S+ → Segnale positivo
- S− → Segnale negativo

Al variare del peso misurato, varia la differenza di volt tra S+ e S-. L'ordine di grandezza è dei milliVolt.
La "Output Sensitivity" della cella (nel nostro caso 1.0 mV/V) indica quanti millivolt la cella produce in uscita per ogni volt di alimentazione, quando è sottoposta al carico massimo nominale (nel nostro caso 50kg)

La variazione è lineare rispetto al peso:
- 0kg su 50kg max -> nessuna deformazione -> 0 mV * Output Sensitivity = 0 mv/V per ogni V
- 25Kg su 50kg max -> metà deformazione -> 0.5 * Output Sensitivity = 0.5 mv/V per ogni V
- 50kg su 50kg max -> deformazione massima -> 1 * Output Sensitivity = 1.0 mv/V per ogni V


## Esempio:
4 celle di carico ognuna da max 50kg, sensibilità di 1.0 mV/V, alimentate con 5V:
### Caso 1: nessun peso (0kg totali, 0kg su ogni cella):
ogni cella dà: 0 mv/V * 5V = 0mv
ponte di Wheatstone: somma differenziale delle 4 celle → uscita finale del ponte = 0mv

### Caso 2: metà peso (100kg totali, 25kg su ogni cella):
ogni cella dà: 0.5 mv/V * 5V = 2.5mv
ponte di Wheatstone: somma differenziale delle 4 celle → uscita finale del ponte = 2.5mv

### Caso 3: massimo peso (200kg totali, 50kg su ogni cella):
ogni cella dà: 1.0 mv/V * 5V = 5mv
ponte di Wheatstone: somma differenziale delle 4 celle → uscita finale del ponte = 5mv

## Amplificazione

Le celle producono un segnale analogico (in tensione continua DC) differenziale in millivolt.
Differenziale: non c'è un riferimento a massa, ma due fili (es. S+ e S−) che vanno letti in differenza

Per poterlo gestire serve amplificarlo. Entra in scena l'**HX711** ovvero un:
- amplificatore a basso rumore,
- convertitore ADC (analogico-digitale) a 24 bit,
- progettato specificamente per celle di carico.

Cosa fa:
- Amplifica il segnale mV → Volt.
- Lo campiona e lo converte in un numero digitale.
- Fornisce questo numero via interfaccia digitale (clock + data), leggibile da un microcontrollore (es. ESP8266, ESP32, Arduino, ..).

### Clock (PIN SCK) e dati (PIN DT)

L’HX711 non ha un clock interno fisso per l'invio dei bit.
Quindi è completamente passivo e segue la velocità dell’altro lato (ESP, Raspberry Pi, STM32, ecc.), anche se richiede che il clock sia lento abbastanza per garantire la stabilità del dato (non si possono mandare colpi da Ghz..). Il suo datasheet raccomanda un massimo di 50 kHz sul pin SCK, ma anche a 10–20 kHz funziona bene.


Il microcontrollore invia quindi impulsi (da LOW a HIGH) sul pin SCK.
Ad ogni colpo di clock, l'HX711 restituisce un bit sul pin DT (data).
Dopo 24 bit, come da specifiche, si è ricomposto il dato grezzo.
Il protocollo prosegue con altri bit ma non ci interessa.

Il protocollo è gestito dalle varie libreria per l'HX711 di ESPHome, Arduino o altri. Non occorre conoscerlo nel dettaglio.

### Valore letto

Il dato letto da HX711 è un numero grezzo (es. -837246 o 1492038) che non rappresenta nessuna grandezza fisica.
E' un intero con segno a 24 bit (da -8.388.608 a +8.388.607) ed è un numero proporzionale alla tensione differenziale tra i pin S+ e S- (output del ponte di Wheatstone). E' un offset, una base per la calibrazione.
Il riferimento di alimentazione (E+ e E−) influenza la scala.

## Calibrazione

Una volta ottenuto il dato grezzo, va fatta la calibrazione per ottenere i Kg o i Grammi.
Il metodo usato è una calibrazione lineare a due punti => è necessario misurare il valore grezzo corrispondente a due pesi noti (di cui uno è tipicamente 0 Kg) e fornire questi due punti all'ESPHome, che userà una formula lineare per calcolare tutti i pesi intermedi

Passaggi teorici:

1. Leggiamo il valore a vuoto → *offset* (esempio: -481989)
2. Mettiamo un peso noto (10kg) e leggiamo il valore → *valore_known* (esempio: 1339163)
3. scale = (valore_known - offset) / peso_noto
4. (ad ogni lettura) peso = (valore_lettura - offset) / scale

Passaggi pratici con ESPHome:

- premere il pulsante "Calibra zero iniziale". Si vedrà il valore di *calib_raw0_diagnostic* modificarsi.
- posizionare un peso noto, più alto possibile, impostare il peso nel campo "Peso noto (kg)" e premere il tasto "Calibra con peso noto". Si vedrà il valore di *calib_raw1_diagnostic* e *calib_weight1_diagnostic* modificarsi.
- da questo momento il valore *scale_value* è calibrato e segna i kg sulla bilancia
