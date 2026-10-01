# TP — Liste chaînée en C

Correction pédagogique du TP « Sortir du fichier unique », réalisée à partir de
l'énoncé fourni et avec l'assistance de Codex.

Le projet illustre la séparation entre interface et implémentation, la
compilation séparée avec `make`, le suivi des allocations et la détection
d'erreurs mémoire.

## Compiler

Avec GCC et GNU Make :

```sh
make clean
make
./demo
```

Sous Windows, l'exécutable peut être lancé avec `./demo.exe`.

## Fichiers

- `liste.h` : interface publique du module ;
- `liste.c` : implémentation de la liste ;
- `main.c` : démonstration des fonctions ;
- `Makefile` : compilation séparée ;
- `deborde.c` : exemple volontairement incorrect pour AddressSanitizer ;
- `COMPTE_RENDU.md` : observations et réponses aux exercices.

