# Herramientas varias

## Afterburner parser

Para sacar la información de un log de afterburner, haz

```bash
python ./afterburner_parser.py -i {{log}}
```

Por defecto, el nombre del log es `HardwareMonitoring.hml`, así que la orden sería `python ./afterburner_parser.py -i ./HardwareMonitoring.hml`