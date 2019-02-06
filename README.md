This is 3.18.117 MT6580 kernel source ported to be used on INFINIX HOT2 x510.

## Known information
| Subsystem | Driver name | Availability | Working |
|-----------|-------------|--------------|---------|
| LCM driver #1 | `xc_rm68200_dsi_vdo_hd720` | Yes | Yes |
| Touch panel | `GT9XX (i2c 1-005D)` | Yes | Yes |
| Charge | `NCP1851 (i2c 1-006A)` | Yes | Yes |
| GPU | `Mali-400 MP` | Yes | Yes |
| Camera #1 | `ov5648_mipi_raw` | Yes | No |
| Camera #2 | `gc2355_mipi_raw` | Yes | No |
| Accelerometer | `MXC662X (i2c 2-0018)` | Yes | No |
| ALS/PS | `LTR559 (i2c 2-0060)` | Yes | No |
| Flash | `Samsung R823MB` | Yes | No |
| Lens #1 | `DW9714AF ` | Yes | No |
| Lens #2 | `FM50AF ` | Yes | No |
| RAM | `2GB & 1GB  LPDDR3_1066` | - | Yes |
| Sound | `mtsndcard` | - | No |
| Accdet | `mt6580-accdet` | - | Yes |
| Other | `kd_camera_hw (i2c 0-0010)` | - | Yes |
| Other | `kd_camera_hw_bus2 (i2c 0-0036)` | - | Yes |


Bugs :

* Front camera
* Upside down back camera
* Volume down button
* GPS


## Acknowledgements

(in alphabetical order)

* Vasya Kovalev [Vgdn1942 (4pda.ru)](https://4pda.ru/forum/index.php?showuser=2214676) [(@Vgdn1942)](https://github.com/Vgdn1942)
* [aleha.druga (4pda.ru)](https://4pda.ru/forum/index.php?showuser=3708916) [(@aleha-druga)](https://github.com/aleha-druga)
* [kva3ar (4pda.ru)](https://4pda.ru/forum/index.php?showuser=6751930)
* [nik-kst (4pda.ru)](https://4pda.ru/forum/index.php?showuser=4052130) [(@nik124seleznev)](https://github.com/nik124seleznev)
* [Skyrimus (4pda.ru)](https://4pda.ru/forum/index.php?showuser=3927665) [(@Skyrimus)](https://github.com/Skyrimus)
* Ibrahim Fathelbab [ibrahim1973 (4pda.ru)](https://4pda.ru/forum/index.php?showuser=8068515) [(@ibrahim1973)](https://github.com/ibrahim1973)
* Yuvraj Saxena [Yuvraj (telegram)](https://t.me/imyuvraj)[(@rad0n)](https://github.com/rad0n)
