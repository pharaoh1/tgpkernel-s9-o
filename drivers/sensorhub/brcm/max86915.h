#ifndef _HRM_MAX86915_H_
#define _HRM_MAX86915_H_

#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/input.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/interrupt.h>
#include <linux/leds.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/gpio.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/file.h>
#include <linux/syscalls.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pm_qos.h>
#ifdef CONFIG_OF
#include <linux/of_gpio.h>
#endif

/*MAX86915 Register Map*/

#define MAX86915_INTERRUPT_STATUS		0x00
#define PWR_RDY_MASK				0x01
#define PROX_INT_MASK				0x10
#define ALC_OVF_MASK				0x20
#define PPG_RDY_MASK				0x40
#define A_FULL_MASK				0x80

#define MAX86915_INTERRUPT_STATUS_2		0x01

#define MAX86915_INTERRUPT_ENABLE		0x02
#define MAX86915_INTERRUPT_ENABLE_2		0x03

#define MAX86915_FIFO_WRITE_POINTER		0x04
#define MAX86915_OVF_COUNTER			0x05
#define MAX86915_FIFO_READ_POINTER		0x06
#define MAX86915_FIFO_DATA			0x07

#define MAX86915_FIFO_CONFIG			0x08
#define MAX86915_FIFO_A_FULL_MASK		0x0F
#define MAX86915_FIFO_A_FULL_OFFSET		0x00
#define MAX86915_FIFO_ROLLS_ON_MASK		0x10
#define MAX86915_FIFO_ROLLS_ON_OFFSET	0x04
#define MAX86915_SMP_AVE_MASK			0xE0
#define MAX86915_SMP_AVE_OFFSET			0x05

#define MAX86915_MODE_CONFIGURATION		0x09
#define MAX86915_MODE_MASK			0x03
#define MAX86915_MODE_OFFSET			0x00
#define MAX86915_AMB_LIGHT_DET_MASK		0x04
#define MAX86915_AMB_LIGHT_DET_OFFSET		0x02
#define MAX86915_RESET_MASK			0x40
#define MAX86915_RESET_OFFSET			0x06
#define MAX86915_SHDN_MASK			0x80
#define MAX86915_SHDN_OFFSET			0x07

#define MAX86915_MODE_CONFIGURATION_2		0x0A
#define MAX86915_LED_PW_MASK			0x03
#define MAX86915_LED_PW_OFFSET			0x00
#define MAX86915_SR_MASK			0x1C
#define MAX86915_SR_OFFSET			0x02
#define MAX86915_ADC_RGE_MASK			0x60
#define MAX86915_ADC_RGE_OFFSET			0x05

#define MAX86915_LED1_PA			0x0C
#define MAX86915_LED2_PA			0x0D
#define MAX86915_LED3_PA			0x0E
#define MAX86915_LED4_PA			0x0F

#define MAX86915_LED_RANGE			0x11
#define MAX86915_LED1_RGE_MASK			0x03
#define MAX86915_LED1_RGE_OFFSET		0x00
#define MAX86915_LED2_RGE_MASK			0x0C
#define MAX86915_LED2_RGE_OFFSET		0x02
#define MAX86915_LED3_RGE_MASK			0x30
#define MAX86915_LED3_RGE_OFFSET		0x04
#define MAX86915_LED4_RGE_MASK			0xC0
#define MAX86915_LED4_RGE_OFFSET		0x06

#define MAX86915_PILOT_PA			0x12

#define MAX86915_LED_SEQ_REG_1			0x13
#define MAX86915_LED1_MASK			0x0F
#define MAX86915_LED1_OFFSET			0x00
#define MAX86915_LED2_MASK			0xF0
#define MAX86915_LED2_OFFSET			0x04

#define MAX86915_LED_SEQ_REG_2			0x14
#define MAX86915_LED3_MASK			0x0F
#define MAX86915_LED3_OFFSET			0x00
#define MAX86915_LED4_MASK			0xF0
#define MAX86915_LED4_OFFSET			0x04

#define MAX86915_DAC1_XTALK_CODE		0x26
#define MAX86915_DAC2_XTALK_CODE		0x27
#define MAX86915_DAC3_XTALK_CODE		0x28
#define MAX86915_DAC4_XTALK_CODE		0x29

#define MAX86915_REV_ID_REG			0xFE
#define MAX86915_PART_ID_REG			0xFF

/* Fixed Value Definition */
#define IR_LED_CH		1
#define RED_LED_CH		2
#define GREEN_LED_CH	3
#define BLUE_LED_CH		4

#define NUM_BYTES_PER_SAMPLE			3
#define MAX_LED_NUM						4

#define MAX86915_COUNT_MAX				65532
#define MAX86915_FIFO_SIZE				32

#define MAX86915_READ_REGFILE_PATH "/data/HRM/MAX86915_READ_REG.txt"
#define MAX86915_WRITE_REGFILE_PATH "/data/HRM/MAX86915_WRITE_REG.txt"

#define I2C_WRITE_ENABLE		0x00
#define I2C_READ_ENABLE			0x01

#define PWR_ON		1
#define PWR_OFF		0
#define PM_RESUME	1
#define PM_SUSPEND	0
#define NAME_LEN		32
#define MAX_BUF_LEN	512

#define MAX_EOL_RESULT 256

/* End of Line Test */
#define SELF_IR_CH		0
#define SELF_RED_CH		1
#define SELF_GREEN_CH		2
#define SELF_BLUE_CH		3
#define SELF_MAX_CH_NUM		4

#define CONVER_FLOAT		65536

#define INTEGER_LEN			16
#define DECIMAL_LEN			10

#define SELF_SQ1_DEFAULT_SPO2		0x07
#define MAX_I2C_BLOCK_SIZE		252

#define SELF_MODE_400HZ		0
#define SELF_MODE_100HZ		1

#define SELF_DIVID_400HZ	1
#define SELF_DIVID_100HZ	4


/* #define HRM_DBG */
/* #define HRM_INFO */

#ifdef HRM_DBG
#define HRM_dbg(format, arg...)		\
				printk(KERN_DEBUG "HRM_dbg : "format, ##arg);
#else
#define HRM_dbg(format, arg...)		{if (hrm_debug)\
				printk(KERN_DEBUG "HRM_dbg : "format, ##arg);\
					}
#endif

#ifdef HRM_INFO
#define HRM_info(format, arg...)	\
				printk(KERN_INFO "HRM_info : "format, ##arg);
#else
#define HRM_info(format, arg...)	{if (hrm_info)\
				printk(KERN_INFO "HRM_info : "format, ##arg);\
					}
#endif

enum _PART_TYPE {
	PART_TYPE_NONE			= 0,
	PART_TYPE_MAX86915_ES		= 30,
	PART_TYPE_MAX86915_CS_15	= 31,
	PART_TYPE_MAX86915_CS_21	= 32,
	PART_TYPE_MAX86915_CS_22	= 33,
	PART_TYPE_MAX86915_CS_211	= 34,
	PART_TYPE_MAX86915_CS_221	= 35,
} PART_TYPE;

enum _AWB_CONFIG {
	AWB_CONFIG0 = 0,
	AWB_CONFIG1,
	AWB_CONFIG2,
} AWB_CONFIG;

enum {
	LED1_UP		= 1 << 0,
	LED1_DOWN	= 1 << 1,
	LED2_UP		= 1 << 2,
	LED2_DOWN	= 1 << 3,
	LED1_SET	= 1 << 4,
	LED2_SET	= 1 << 5,
} LED_CON;

static enum {
	S_INIT = 0,
	S_F_A,
	S_CAL,
	S_F_D,
	S_ERR,
	S_UNKNOWN
} agc_pre_s, agc_cur_s;

enum {
	DEBUG_WRITE_REG_TO_FILE = 1,
	DEBUG_WRITE_FILE_TO_REG = 2,
	DEBUG_SHOW_DEBUG_INFO = 3,
	DEBUG_ENABLE_AGC = 4,
	DEBUG_DISABLE_AGC = 5,
};

enum {
	M_HRM,
	M_HRMLED_IR,
	M_HRMLED_RED,
	M_HRMLED_BOTH,
	M_SDK,
	M_NONE
};

enum _EOL_STATE_TYPE_NEW {
	_EOL_STATE_TYPE_NEW_INIT = 0,
	_EOL_STATE_TYPE_NEW_FLICKER_INIT, /* step 1. flicker_step. */
	_EOL_STATE_TYPE_NEW_FLICKER_MODE,
	_EOL_STATE_TYPE_NEW_DC_INIT,      /* step 2. dc_step. */
	_EOL_STATE_TYPE_NEW_DC_MODE_LOW,
	_EOL_STATE_TYPE_NEW_DC_MODE_MID,
	_EOL_STATE_TYPE_NEW_DC_MODE_HIGH,
	_EOL_STATE_TYPE_NEW_DC_XTALK,
	_EOL_STATE_TYPE_NEW_FREQ_INIT,   /* step 3. freq_step. */
	_EOL_STATE_TYPE_NEW_FREQ_MODE,
	_EOL_STATE_TYPE_NEW_END,
	_EOL_STATE_TYPE_NEW_STOP,
} _EOL_STATE_TYPE_NEW;

enum hrm_mode {
	MODE_NONE			= 0,
	MODE_HRM			= 1,
	MODE_AMBIENT		= 2,
	MODE_PROX			= 3,
	MODE_HRMLED_IR		= 4,
	MODE_HRMLED_RED		= 5,
	MODE_HRMLED_BOTH	= 6,
	MODE_SDK_IR			= 10,
	MODE_SDK_RED		= 11,
	MODE_SDK_GREEN		= 12,
	MODE_SDK_BLUE		= 13,
	MODE_UNKNOWN		= 14,
};

/* to protect the system performance issue. */
union int_status {
	struct {
		struct {
			unsigned char pwr_rdy:1;
			unsigned char:1;
			unsigned char:1;
			unsigned char uv_rdy:1;
			unsigned char prox_int:1;
			unsigned char alc_ovf:1;
			unsigned char ppg_rdy:1;
			unsigned char a_full:1;
		};
		struct {
			unsigned char:1;
			unsigned char die_temp_rdy:1;
			unsigned char ecg_imp_rdy:1;
			unsigned char:3;
			unsigned char blue_rdy:1;
			unsigned char vdd_oor:1;
		};
	};
	u8 val[2];
};

struct max86915_div_data {
	char left_integer[INTEGER_LEN];
	char left_decimal[DECIMAL_LEN];
	char right_integer[INTEGER_LEN];
	char right_decimal[DECIMAL_LEN];
	char result_integer[INTEGER_LEN];
	char result_decimal[DECIMAL_LEN];
};

/* step flicker mode */
struct  max86915_eol_flicker_data {
	int count;
	int state;
	int retry;
	int index;
	u64 max;
	u64 buf[SELF_RED_CH][EOL_NEW_FLICKER_SIZE];
	u64 sum[SELF_RED_CH];
	u64 average[SELF_RED_CH];
	u64 std_sum[SELF_RED_CH];
	int done;
};
/* step dc level */
struct  max86915_eol_hrm_data {
	int count;
	int index;
	int ir_current;
	int red_current;
	int green_current;
	int blue_current;
	u64 buf[SELF_MAX_CH_NUM][EOL_NEW_HRM_SIZE];
	u64 sum[SELF_MAX_CH_NUM];
	u64 average[SELF_MAX_CH_NUM];
	u64 std_sum[SELF_MAX_CH_NUM];
	int done;
};
/* step freq_step */
struct  max86915_eol_freq_data {
	int state;
	int retry;
	int skew_retry;
	unsigned long end_time;
	unsigned long start_time;
	struct timeval start_time_t;
	struct timeval work_time_t;
	unsigned long prev_time;
	int count;
	int skip_count;
	int done;
};

struct  max86915_eol_data {
	int eol_count;
	u8 state;
	int eol_hrm_flag;
	struct timeval start_time;
	struct timeval end_time;
	struct  max86915_eol_flicker_data eol_flicker_data;
	struct  max86915_eol_hrm_data eol_hrm_data[EOL_NEW_HRM_ARRY_SIZE];
	struct  max86915_eol_freq_data eol_freq_data;	/* test the ir routine. */
};

struct hrm_enable_count {
	s32 hrm_cnt;
	s32 amb_cnt;
	s32 prox_cnt;
	s32 sdk_cnt;
	s32 cgm_cnt;
	s32 unkn_cnt;
};

struct hrm_output_data {
	enum hrm_mode mode;
	s32 main_num;
	s32 data_main[6];
	s32 sub_num;
	s32 data_sub[8];
	s32 fifo_main[4][32];
	s32 fifo_num;
};

/* max86915 Data Structure */
struct max869_device_data {
#ifdef CONFIG_SPI_TO_I2C_FPGA
	struct platform_device *pdev;
#endif
	struct i2c_client *client;
	struct device *dev;
	struct input_dev *hrm_input_dev;
	struct mutex i2clock;
	struct mutex activelock;
	struct mutex suspendlock;
	struct mutex flickerdatalock;
	struct miscdevice miscdev;
	struct pinctrl *hrm_pinctrl;
	struct pinctrl_state *pins_sleep;
	struct pinctrl_state *pins_idle;
	char *led_3p3;
	char *vdd_1p8;
	char *i2c_1p8;
	enum hrm_mode hrm_enabled_mode;
	enum hrm_mode hrm_prev_mode;
	u32 sampling_period_ns;
	u8 mode_sdk_enabled;
	u8 mode_sdk_prev;
	u8 regulator_state;
	s32 hrm_int;
	s32 hrm_en;
	s32 hrm_irq;
	u8 irq_state;
	u32 led_current;
	u32 xtalk_code;
	s32 hrm_threshold;
	s32 eol_test_is_enable;
	u8 eol_test_status;
	s32 pre_eol_test_is_enable;
	u32 reg_read_buf;
	u8 debug_mode;
	struct hrm_enable_count mode_cnt;
	char *lib_ver;
#ifdef CONFIG_ARCH_QCOM
	struct pm_qos_request pm_qos_req_fpm;
#endif
	bool pm_state;

	s32 hrm_mode;
	u8 agc_mode;
	char *eol_test_result;
	u8 part_type;
	u8 flex_mode;
	s32 num_samples;
	u16 sample_cnt;
	u8 awb_flicker_status;
	u8 led_current1;
	u8 led_current2;
	u8 led_current3;
	u8 led_current4;
	u8 default_current1;
	u8 default_current2;
	u8 default_current3;
	u8 default_current4;

	u8 xtalk_code1;
	u8 xtalk_code2;
	u8 xtalk_code3;
	u8 xtalk_code4;
	u8 default_xtalk_code1;
	u8 default_xtalk_code2;
	u8 default_xtalk_code3;
	u8 default_xtalk_code4;

	u16 awb_sample_cnt;
	int *flicker_data;
	int flicker_data_cnt;

	/* AGC routine start */
	u8 ir_led;
	u8 red_led;
	u8 green_led;
	u8 blue_led;

	bool agc_is_enable;
	int agc_sum[4];
	u32 agc_current[4];
	u8 agc_led_set;

	s32 agc_led_out_percent;
	s32 agc_corr_coeff;
	s32 agc_min_num_samples;
	s32 agc_sensitivity_percent;
	s32 change_by_percent_of_range[4];
	s32 change_by_percent_of_current_setting[4];
	s32 change_led_by_absolute_count[4];
	s32 reached_thresh[4];
	s32 threshold_default;

	int (*update_led)(struct max869_device_data*, int, int);
	/* AGC routine end */

	struct max86915_eol_data eol_data;

	int isTrimmed;

	u32 i2c_err_cnt;
	u16 ir_curr;
	u16 red_curr;
	u16 green_curr;
	u16 blue_curr;
	u32 ir_adc;
	u32 red_adc;
	u32 green_adc;
	u32 blue_adc;
	u32 agc_ch_adc[4];

	int agc_enabled;
	int fifo_data[MAX_LED_NUM][MAX86915_FIFO_SIZE];
	int fifo_samples;
	int vdd_oor_cnt;
	u16 agc_sample_cnt[4];

	int prev_ppg[MAX_LED_NUM];
	int prev_current[MAX_LED_NUM];
	int prev_agc_current[MAX_LED_NUM];
};

extern int sensors_create_symlink(struct input_dev *inputdev);
extern void sensors_remove_symlink(struct input_dev *inputdev);
#ifdef CONFIG_ARCH_QCOM
extern int sensors_register(struct device **dev, void *drvdata,
	struct device_attribute *attributes[], char *name);
#else
extern int sensors_register(struct device *dev, void *drvdata,
	struct device_attribute *attributes[], char *name);
#endif
extern void sensors_unregister(struct device *dev,
	struct device_attribute *attributes[]);

extern unsigned int lpcharge;

#endif /* _HRM_MAX86915_H_ */
