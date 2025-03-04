#ifndef TLX493D_GEN_3_CONFIG_COMMON_H
#define TLX493D_GEN_3_CONFIG_COMMON_H


/** DS v1.00 */
#define GEN_3_O0x	52.46501931L
#define GEN_3_O1x	-30.828402e-3L
#define GEN_3_O2x	6.06444e-6L
#define GEN_3_O3x	-4.20546e-10L
#define GEN_3_L0x	-2.109359211L
#define GEN_3_L1x	2.248525e-3L
#define GEN_3_L2x	-5.25818e-7L
#define GEN_3_L3x	3.99648e-11L
#define GEN_3_O0y	7.574714985L
#define GEN_3_O1y	-4.602293e-3L
#define GEN_3_O2y	8.61016e-7L
#define GEN_3_O3y	-7.47545e-11L
#define GEN_3_L0y	-2.106808409L
#define GEN_3_L1y	2.234594e-3L
#define GEN_3_L2y	-5.22864e-7L
#define GEN_3_L3y	3.97614e-11L
#define GEN_3_O0z	9.233258372L
#define GEN_3_O1z	-3.911673e-3L
#define GEN_3_O2z	7.01838e-7L
#define GEN_3_O3z	-4.38542e-11L
#define GEN_3_L0z	-0.96458813L
#define GEN_3_L1z	1.445091e-3L
#define GEN_3_L2z	-3.42739e-7L
#define GEN_3_L3z	2.63e-11L


/** DS v0.60 */
/**
#define GEN_3_O0x	17.3267854124448L
#define GEN_3_O1x	-7.35697488204487e-3L
#define GEN_3_O2x	9.04386700139679e-7L
#define GEN_3_O3x	-4.11136942302164e-11L
#define GEN_3_L0x	-1.97791738051192L
#define GEN_3_L1x	2.16084455894181e-3L 
#define GEN_3_L2x	-5.11108553761453e-7L 
#define GEN_3_L3x	3.92456873185757e-11L 
#define GEN_3_O0y	13.1429984746861L
#define GEN_3_O1y	-6.60898501857681e-3L 
#define GEN_3_O2y	1.01138132545504e-6L 
#define GEN_3_O3y	-6.48339760449271e-11L 
#define GEN_3_L0y	-1.92651899997075L
#define GEN_3_L1y	2.12764348620027e-3L 
#define GEN_3_L2y	-5.03546107645159e-7L 
#define GEN_3_L3y	3.86667828787361e-11L 
#define GEN_3_O0z	11.7678191529052L
#define GEN_3_O1z	-9.99695732514966e-3L 
#define GEN_3_O2z	2.54072819527595e-6L 
#define GEN_3_O3z	-2.04419211153673e-10L 
#define GEN_3_L0z	-0.912964983702195L
#define GEN_3_L1z	1.41770522399259e-3L 
#define GEN_3_L2z	-3.38806085881603e-7L 
#define GEN_3_L3z	2.61297188837947e-11L
*/

/** DS v1.00 */
#define GEN_3_FULL_RANGE_FIELD_SENSITIVITY         29.5

/** DS v0.60 */
/** #define GEN_3_FULL_RANGE_FIELD_SENSITIVITY         30.5 */
/** #define GEN_3_SHORT_RANGE_FIELD_SENSITIVITY        61.0 */
/** #define GEN_3_EXTRA_SHORT_RANGE_FIELD_SENSITIVITY  122.0 */

#define GEN_3_TEMP_SENSITIVITY          15.2
#define GEN_3_TEMP_OFFSET               4200.0
#define GEN_3_TEMP_REF                  25.0

/** IIC address. */
/** A0 */
#define GEN_3_STD_IIC_ADDR_WRITE_A0     0xBA
#define GEN_3_STD_IIC_ADDR_READ_A0      0xBB

/** A1 */
#define GEN_3_STD_IIC_ADDR_WRITE_A1     0x26
#define GEN_3_STD_IIC_ADDR_READ_A1      0x27

/** A2 */
#define GEN_3_STD_IIC_ADDR_WRITE_A2     0x52
#define GEN_3_STD_IIC_ADDR_READ_A2      0x53

/** A3 */
#define GEN_3_STD_IIC_ADDR_WRITE_A3     0x8C
#define GEN_3_STD_IIC_ADDR_READ_A3      0x8D


#define GEN_3_REG_MAP_SIZE              27


#endif /** TLX493D_GEN_3_CONFIG_COMMON_H */
