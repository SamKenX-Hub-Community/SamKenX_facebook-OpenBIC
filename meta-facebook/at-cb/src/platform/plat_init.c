/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "hal_gpio.h"
#include "hal_peci.h"
#include "power_status.h"
#include "util_sys.h"
#include "plat_gpio.h"
#include "plat_mctp.h"
#include "plat_class.h"
#include "plat_i2c_target.h"
#include "util_worker.h"

SCU_CFG scu_cfg[] = {
	//register    value
};

void pal_pre_init()
{
	/* init i2c target */
	for (int index = 0; index < MAX_TARGET_NUM; index++) {
		if (I2C_TARGET_ENABLE_TABLE[index])
			i2c_target_control(
				index, (struct _i2c_target_config *)&I2C_TARGET_CONFIG_TABLE[index],
				1);
	}

	check_asic_card_status();
	init_plat_worker(CONFIG_MAIN_THREAD_PRIORITY + 1); // work queue for low priority jobs
}

void pal_post_init()
{
	plat_mctp_init();
}

void pal_device_init()
{
	return;
}

void pal_set_sys_status()
{
	gpio_set(ACB_BIC_READY_N, GPIO_LOW);
	return;
}

#define DEF_PROJ_GPIO_PRIORITY 78

DEVICE_DEFINE(PRE_DEF_PROJ_GPIO, "PRE_DEF_PROJ_GPIO_NAME", &gpio_init, NULL, NULL, NULL,
	      POST_KERNEL, DEF_PROJ_GPIO_PRIORITY, NULL);
