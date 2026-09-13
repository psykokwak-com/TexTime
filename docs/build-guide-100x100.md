# Fabriquer une TexTime 100 × 100

L'horloge d'un mètre. Elle reprend l'électronique du 40 × 40 en version bandeau ; la
mécanique change entièrement : pas de cadre ni d'impression 3D, la structure est un
sandwich de plaques de MDF découpées au laser et collées, sur lequel vient la façade.

Ce guide décrit les fichiers disponibles et les différences avec le 40 × 40. Pour tout
ce qui est commun, se reporter au [guide 40 × 40](build-guide-40x40.md).

Nomenclature : [`bom/100x100.md`](../bom/100x100.md).

## 1. La façade

Acier S235 de 1 mm, 90 × 90 cm, découpée au laser :
[`faceplate-steel-s235-1mm.dxf`](../hardware/mechanical/100x100/faceplate-steel-s235-1mm.dxf).
Même démarche qu'au chapitre 1 du guide 40 × 40, mais à cette taille il faut un
sous-traitant équipé en grand format, et la pièce se fait livrer. Finition rouillée et
vernie, ou peinte, comme pour le 40 × 40.

## 2. La structure

Cinq plaques de MDF découpées au laser, empilées et collées, forment la structure
complète : elles portent les LED, forment les alvéoles derrière chaque lettre et
reçoivent la façade. Trois dessins, dont un découpé en trois exemplaires :

| Position | Plaque | Épaisseur | Fichier |
|---|---|---|---|
| Avant, côté façade | couche 0 | 3 mm | [`led-support-layer0-mdf-3mm.dxf`](../hardware/mechanical/100x100/led-support-layer0-mdf-3mm.dxf) |
| Milieu, × 3 | couche 1 | 10 mm | [`led-support-layer1-mdf-10mm.dxf`](../hardware/mechanical/100x100/led-support-layer1-mdf-10mm.dxf) |
| Arrière | couche 2 | 3 mm | [`led-support-layer2-mdf-3mm.dxf`](../hardware/mechanical/100x100/led-support-layer2-mdf-3mm.dxf) |

Soit 1 + 3 × 1 + 1 plaques, 36 mm d'épaisseur au total. L'assemblage, pour se
représenter l'ensemble :
[`led-support-assembly.stl.zip`](../hardware/mechanical/100x100/led-support-assembly.stl.zip).

Coller les plaques les unes sur les autres en les alignant sur leurs découpes, sous
presse ou sous poids le temps du séchage.

## 3. L'électronique

Le circuit de contrôle est la carte `pcb-multi-ledstripe`, sans changement : voir le
chapitre 3.1 du [guide 40 × 40](build-guide-40x40.md#31-le-circuit-de-contrôle).

### 3.1 Le bandeau

Privilégier un bandeau **SK6812 RGBW 5 V**, 30 LED/m, IP30 : de meilleure qualité que
les WS2812/WS2813, avec une LED blanche dédiée que le firmware utilise pour le texte
blanc. Un bandeau WS2813 fonctionne aussi. Dans les deux cas, le type se choisit dans
les paramètres généraux, « Type de LED ».

Le bandeau parcourt les dix lignes de lettres en boustrophédon, deux LED par lettre. Le
firmware connaît trois câblages, à choisir dans « Type d'horloge » :

| Type d'horloge | Câblage | LED par lettre | LED au total |
|---|---|---|---|
| `100x100@3` | **Bandeau plié** en bout de ligne pour repartir sur la suivante. Aucune découpe, aucun raccord ; les 4 LED du pli restent éteintes. Le plus rapide à assembler, recommandé. | 2 | 262 |
| `100x100@2` | Bandeau coupé à chaque ligne, sections reliées par câble ; 2 LED inutilisées entre deux lignes | 2 | 244 |
| `100x100@1` | Bandeau coupé à chaque ligne, sections reliées par câble ; 1 LED inutilisée entre deux lignes | 1 | 234 |

Avec le câblage plié, il faut environ 8,8 m de bandeau à 30 LED/m, soit deux rouleaux
de 5 m. L'ordre exact des LED de chaque câblage est dans
[`LedStrip.h`](../firmware/TexTime/LedStrip.h), classes `LedConfiguration100x100_1`, `_2` et `_3`.

### 3.2 L'alimentation

Avec deux fois plus de LED qu'en 40 × 40, l'alimentation passe à 5 V 10 A. Reboucler le
+5 V et le GND en fin de bandeau est encore plus nécessaire sur cette longueur.

## 4. Firmware, assemblage, mise en route

Identiques au 40 × 40, chapitres 4 à 6 du [guide](build-guide-40x40.md). Deux
différences dans les paramètres généraux : « Type d'horloge » sur le câblage
`100x100@…` correspondant au bandeau posé, et « Type de LED » sur le bandeau choisi.
