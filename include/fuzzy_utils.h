// includi solo una volta
#pragma once

// fuzzy_utils.h
#ifndef FUZZY_UTILS_H
#define FUZZY_UTILS_H

#include <math.h>


/// funzione triangolare
inline float triangle(float x, float a, float b, float c) {
  if (x <= a || x >= c) return 0.0;
  else if (x == b) return 1.0;
  else if (x < b) return (x - a) / (b - a);
  else return (c - x) / (c - b);
}


/// funzione trapezoidale
inline float trapezoid(float x, float a, float b, float c, float d) {
  if (x <= a || x >= d) return 0.0;
  else if (x >= b && x <= c) return 1.0;
  else if (x > a && x < b) return (x - a) / (b - a);
  else return (d - x) / (d - c);
}


/// funzione left shoulder
inline float left_shoulder(float x, float a, float b) {
  if (x <= a) return 1.0;
  else if (x >= b) return 0.0;
  else return (b - x) / (b - a);
}


/// funzione right shoulder
inline float right_shoulder(float x, float a, float b) {
  if (x <= a) return 0.0;
  else if (x >= b) return 1.0;
  else return (x - a) / (b - a);
}


/// Curva gaussian
inline float gaussian(float x, float c, float sigma) {
  return exp(-0.5 * pow((x - c) / sigma, 2));
}


/// Curva sigmoid
inline float sigmoid(float x, float a, float c) {
  return 1.0 / (1.0 + exp(-a * (x - c)));
}


/// Fuzzy AND (classico): restituisce il minimo tra due valori fuzzy.
inline float fuzzy_and(float a, float b) {
  return std::min(a, b);
}

/// Fuzzy OR (classico): restituisce il massimo tra due valori fuzzy.
inline float fuzzy_or(float a, float b) {
  return std::max(a, b);
}

/// Fuzzy NOT: inverte un valore fuzzy (es. 1 → 0, 0.3 → 0.7).
inline float fuzzy_not(float a) {
  return 1.0f - a;
}



/// Calcola il valore defuzzificato secondo il metodo MOM (Mean of Maximum)
/// "se è più vero che fa caldo rispetto al freddo, accendi il ventilatore forte"
/// @param values Vettore di valori di output (es: velocità ventola in RPM)
/// @param memberships Vettore dei gradi fuzzy corrispondenti (es: [0.2, 0.8, 0.8, 0.4])
/// @return La media dei valori che hanno il massimo grado di appartenenza
inline float fuzzy_mean_of_maximum(const std::vector<float>& values, const std::vector<float>& memberships) {
  if (values.size() != memberships.size() || values.empty()) return 0.0f;

  float max_membership = *std::max_element(memberships.begin(), memberships.end());
  float sum = 0.0f;
  int count = 0;

  for (size_t i = 0; i < values.size(); ++i) {
      if (memberships[i] == max_membership) {
          sum += values[i];
          count++;
      }
  }

  return (count > 0) ? (sum / count) : 0.0f;
}



/// Calcola il centroide (media ponderata) tra valori fuzzy e corrispondenti output reali.
/// "dato che è la temp è poco bassa, abbastanza media e un po' alta, trova un compromesso tra tutte"
/// @tparam N Numero di regole.
/// @param fuzzy_values Array di appartenenze fuzzy (tra 0.0 e 1.0)
/// @param output_values Array dei corrispondenti output reali (es: RPM, °C, ecc.)
/// @return Valore defuzzificato reale. Se somma fuzzy = 0, ritorna 0.
template <size_t N>
inline float defuzzify_centroid(const std::array<float, N>& fuzzy_values, const std::array<float, N>& output_values) {
    float num = 0.0f;
    float denom = 0.0f;

    for (size_t i = 0; i < N; ++i) {
        num += fuzzy_values[i] * output_values[i];
        denom += fuzzy_values[i];
    }

    if (denom == 0.0f) return 0.0f;  // evitare divisione per zero

    return num / denom;
}

#endif


