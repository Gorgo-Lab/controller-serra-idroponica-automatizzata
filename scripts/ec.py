import math


def calcola_ec_da_tensione(
    read_voltage: float,
    temperature_celsius: float
) -> float:
  
  """
    da https://wiki.dfrobot.com/Gravity__Analog_TDS_Sensor___Meter_For_Arduino_SKU__SEN0244

    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0)); 
    float compensationCoefficient=1.0+0.02*(temperature-25.0);

    //temperature compensation
    float compensationVolatge=averageVoltage/compensationCoefficient;

    //convert voltage value to tds value
    tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5;  

  """

  # Calcola il coefficiente di compensazione termica
  compensation_coefficient = 1.0 + 0.02 * (temperature_celsius - 25.0)

  # Applica la compensazione alla tensione letta
  compensation_voltage = read_voltage / compensation_coefficient

  # Calcola il valore di EC usando la formula polinomiale
  ec_value = (
      (math.pow(compensation_voltage, 3) * 133.42) -
      (math.pow(compensation_voltage, 2) * 255.86) +
      (compensation_voltage * 857.39)
  ) * 0.5
  
  return ec_value



def calcola_ec_da_tensione_2(
    read_voltage: float,
    temperature_celsius: float
) -> float:

    # Calcola il coefficiente di compensazione termica
    compensation_coefficient = 1.0 + 0.02 * (temperature_celsius - 25.0)

    # Applica la compensazione alla tensione letta
    compensation_voltage = read_voltage / compensation_coefficient

    # Calcola il valore di EC usando la formula polinomiale
    ec_value = (
        (math.pow(compensation_voltage, 3) * 133.42) -
        (math.pow(compensation_voltage, 2) * 255.86) +
        (compensation_voltage * 857.39)
    ) * 0.5
    #ec_value = 1413

    return compensation_coefficient * ec_value



def calcola_ec_da_tensione_3(
    read_voltage: float,
    temperature_celsius: float
) -> float:
  
  """
    da https://wiki.dfrobot.com/Gravity__Analog_TDS_Sensor___Meter_For_Arduino_SKU__SEN0244

    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0)); 
    float compensationCoefficient=1.0+0.02*(temperature-25.0);

    //temperature compensation
    float compensationVolatge=averageVoltage/compensationCoefficient;

    //convert voltage value to tds value
    tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5;  

  """

  # Calcola il coefficiente di compensazione termica
  compensation_coefficient = 1.0 + 0.02 * (temperature_celsius - 25.0)

  # Applica la compensazione alla tensione letta
  compensation_voltage = read_voltage / compensation_coefficient

  # Calcola il valore di EC usando la formula polinomiale
  tds = (
      (math.pow(compensation_voltage, 3) * 133.42) -
      (math.pow(compensation_voltage, 2) * 255.86) +
      (compensation_voltage * 857.39)
  ) * 0.7
  
  return tds



volt = 1.75650
temperatura = 21.1

tds = calcola_ec_da_tensione_3(volt, temperatura)

print(f"il valore letto in volt: {volt}")
print(f"la temperatura: {temperatura}")
print(f"Il valore di TDS calcolato è: {tds} uS/cm")


"""

il valore letto in volt: 1.7565
la temperatura: 22.1
Il valore di TDS calcolato è: 1101.8845835842146 uS/cm



"""
