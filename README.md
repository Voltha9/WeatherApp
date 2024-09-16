# WeatherApp

WeatherApp est une application C++ qui utilise l'API OpenWeatherMap pour récupérer et afficher des informations météorologiques. L'application rend les données météorologiques telles que la température, le taux d'humidité, la pression atmosphérique et plus encore, grâce à une interface utilisateur graphique construite avec SFML et ImGui.

## Fonctionnalités

- Affichage en temps réel des conditions météorologiques actuelles.
- Interface utilisateur graphique interactive avec SFML et ImGui.
- Utilisation de l'API OpenWeatherMap pour les données météorologiques en temps réel.

## Prérequis

Le script `BuildWeather.bat` simplifie la configuration en installant automatiquement `vcpkg` (si nécessaire) et les bibliothèques nécessaires via `vcpkg`. Assurez-vous que la variable d'environnement `VCPKG_ROOT` est définie si `vcpkg` est déjà installé sur votre système.

**Variables d'environnement importantes :**
- `WEATHER_API_KEY` : Votre clé API pour accéder à OpenWeatherMap. Vous devez obtenir une clé API en créant un compte sur [OpenWeatherMap](https://openweathermap.org/).

**Autres exigences :**
- Microsoft Visual Studio avec le compilateur MSVC.

## Configuration

1. Clonez le dépôt GitHub :
   ```
   git clone https://github.com/Voltha9/WeatherApp.git
   ```
2. Naviguez dans le dossier du projet et exécutez `BuildWeather.bat`. Ce script configurera automatiquement l'environnement de développement, installera les dépendances, et construira le projet. Il vous proposera de choisir entre les modes Debug ou Release pour la compilation.

3. Définissez la variable d'environnement `WEATHER_API_KEY` avec votre clé API OpenWeatherMap avant d'exécuter l'application.

## Compilation et Exécution

Vous n'avez pas besoin de lancer manuellement des commandes CMake pour compiler le projet. Le script `BuildWeather.bat` s'en charge entièrement. Suivez simplement les instructions du script pour compiler et exécuter l'application.

## Contribuer

Les contributions sont les bienvenues. Veuillez créer une branche pour les modifications proposées et soumettre une pull request pour révision.

## Licence

WeatherApp est sous licence MIT.
