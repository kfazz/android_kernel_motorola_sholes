/*
 * motsnd.c  --  SoC audio for Motorola Android Platform
 *
 * Author: Motorola
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <linux/clk.h>
#include <linux/platform_device.h>
//#include <linux/cpcap_audio_platform_data.h>
#include <linux/clk.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/pcm_params.h>

#include <asm/mach-types.h>
#include <plat/hardware.h>
#include <plat/gpio.h>
#include <plat/mcbsp.h>
#include <plat/clock.h>

#include "omap-mcbsp.h"
#include "omap-pcm.h"
#include "../codecs/cpcap.h"

#include "../../../arch/arm/mach-omap2/clock.h"


 #define MOTSND_DEBUG 
#ifdef MOTSND_DEBUG
#define MOTSND_DEBUG_LOG(args...) printk(KERN_INFO "ALSA MOTSND:" args)
#else
#define MOTSND_DEBUG_LOG(args...)
#endif


static int motsnd_hw_params(struct snd_pcm_substream *substream,
			    struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->dai->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->dai->cpu_dai;
	int ret;

	MOTSND_DEBUG_LOG("%s: entered\n", __func__);

	/* Set codec DAI configuration */
	ret = snd_soc_dai_set_fmt(codec_dai,
				  SND_SOC_DAIFMT_I2S |
				  SND_SOC_DAIFMT_NB_IF |
				  SND_SOC_DAIFMT_CBM_CFM);
	if (ret < 0) {
		printk(KERN_ERR "can't set codec DAI configuration\n");
		return ret;
	}

	/* Set cpu DAI configuration */
	ret = snd_soc_dai_set_fmt(cpu_dai,
				  SND_SOC_DAIFMT_I2S |
				  SND_SOC_DAIFMT_IB_NF |
				  SND_SOC_DAIFMT_CBM_CFM);
	if (ret < 0) {
		printk(KERN_ERR "can't set cpu DAI configuration\n");
		return ret;
	}

	/* Set the codec system clock for DAC and ADC */
	ret = snd_soc_dai_set_sysclk(codec_dai, 0, 26000000,
				     SND_SOC_CLOCK_IN);
	if (ret < 0) {
		printk(KERN_ERR "can't set codec system clock\n");
		return ret;
	}

	/* Set cpu DAI configuration */
	ret = snd_soc_dai_set_sysclk(cpu_dai,
				     OMAP_MCBSP_SYSCLK_CLKX_EXT,
				     0, 0);
	if (ret < 0)
		printk(KERN_ERR "can't set cpu DAI system clock\n");

	return ret;
}

static struct snd_soc_ops motsnd_ops = {
	.hw_params = motsnd_hw_params,
};

static int motsnd_voice_hw_params(struct snd_pcm_substream *substream,
				  struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->dai->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->dai->cpu_dai;
	int ret;

	MOTSND_DEBUG_LOG("%s: entered\n", __func__);

	/* Set codec DAI configuration */
	ret = snd_soc_dai_set_fmt(codec_dai,
				  SND_SOC_DAIFMT_DSP_A |
				  SND_SOC_DAIFMT_NB_NF |
				  SND_SOC_DAIFMT_CBM_CFM);
	if (ret) {
		printk(KERN_ERR "can't set codec DAI configuration\n");
		return ret;
	}

	/* Set cpu DAI configuration */
	ret = snd_soc_dai_set_fmt(cpu_dai,
				SND_SOC_DAIFMT_DSP_A |
				SND_SOC_DAIFMT_IB_NF |
				SND_SOC_DAIFMT_CBM_CFM);
	if (ret < 0) {
		printk(KERN_ERR "can't set cpu DAI configuration\n");
		return ret;
	}

	/* Set the codec system clock for DAC and ADC */
	ret = snd_soc_dai_set_sysclk(codec_dai, 0, 26000000,
				     SND_SOC_CLOCK_IN);
	if (ret < 0) {
		printk(KERN_ERR "can't set codec system clock\n");
		return ret;
	}

	/* Set cpu DAI configuration */
	ret = snd_soc_dai_set_sysclk(cpu_dai,
				     OMAP_MCBSP_SYSCLK_CLKX_EXT,
				     0, 0);
	if (ret < 0)
		printk(KERN_ERR "can't set cpu DAI system clock\n");

	return ret;
}

static struct snd_soc_ops motsnd_voice_ops = {
	.hw_params = motsnd_voice_hw_params,
};

static int motsnd_incall_hw_params(struct snd_pcm_substream *substream,
				   struct snd_pcm_hw_params *params)
{
	MOTSND_DEBUG_LOG("%s: entered\n", __func__);

	return 0;
}

static struct snd_soc_ops motsnd_incall_ops = {
	.hw_params = motsnd_incall_hw_params,
};

static int motsnd_fm_hw_params(struct snd_pcm_substream *substream,
			       struct snd_pcm_hw_params *params)
{
	MOTSND_DEBUG_LOG("%s: entered\n", __func__);
	return 0;
}

static struct snd_soc_ops motsnd_fm_ops = {
	.hw_params = motsnd_fm_hw_params,
};

static int motsnd_btvoice_hw_params(struct snd_pcm_substream *substream,
				  struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->dai->cpu_dai;
	struct snd_soc_codec *codec = rtd->dai->codec_dai->codec;
	struct platform_device *pdev = container_of(codec->dev,
			struct platform_device, dev);
	struct cpcap_audio_pdata *pdata = pdev->dev.platform_data;
	int ret;

	MOTSND_DEBUG_LOG("%s: entered\n", __func__);


		/* MDM6600 */
		/* Set cpu DAI configuration */
		ret = snd_soc_dai_set_fmt(cpu_dai,
					  SND_SOC_DAIFMT_I2S |
					  SND_SOC_DAIFMT_IB_IF |
					  SND_SOC_DAIFMT_CBM_CFM);
		if (ret < 0) {
			printk(KERN_ERR "can't set cpu DAI configuration\n");
			return ret;
		}

	/* Set cpu DAI configuration */
	ret = snd_soc_dai_set_sysclk(cpu_dai,
				     OMAP_MCBSP_SYSCLK_CLKX_EXT,
				     0, 0);
	if (ret < 0)
		printk(KERN_ERR "can't set cpu DAI system clock\n");

	return ret;
}

static struct snd_soc_ops motsnd_btvoice_ops = {
	.hw_params = motsnd_btvoice_hw_params,
};

static int mcbsp_be_hw_params_fixup(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params)
{
	struct snd_interval *channels = hw_param_interval(params,
						SNDRV_PCM_HW_PARAM_CHANNELS);
	unsigned int val, min_mask;


	val = SNDRV_PCM_FORMAT_S16_LE;

	min_mask = snd_mask_min(&params->masks[SNDRV_PCM_HW_PARAM_FORMAT -
				SNDRV_PCM_HW_PARAM_FIRST_MASK]);
	snd_mask_reset(&params->masks[SNDRV_PCM_HW_PARAM_FORMAT -
			SNDRV_PCM_HW_PARAM_FIRST_MASK],
			min_mask);
	snd_mask_set(&params->masks[SNDRV_PCM_HW_PARAM_FORMAT -
				    SNDRV_PCM_HW_PARAM_FIRST_MASK],
		     val);
	return 0;
}

static int motsnd_cpcap_init(struct snd_soc_codec *codec)
{
	MOTSND_DEBUG_LOG("%s: Entered\n", __func__);
	return 0;
}

static int motsnd_cpcap_voice_init(struct snd_soc_codec *codec)
{
	MOTSND_DEBUG_LOG("%s: Entered\n", __func__);
	return 0;
}

static struct snd_soc_dai modem_dai = {
	.name = "MODEM",
	.id = 0,
	/*.playback = {
		.stream_name = "VoiceCall-DL",
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_8000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.capture = {
		.stream_name = "VoiceCall-UL",
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_8000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},*/
};

/* Digital audio interface glue - connects codec <--> CPU */
static struct snd_soc_dai_link motsnd_dai[] = {

{
	.name = "HiFi",
	.stream_name = "McBSP2-Codec",
	.cpu_dai = &omap_mcbsp_dai[0],
	.codec_dai = &cpcap_dai[0],
	.init = motsnd_cpcap_voice_init,
	.ops = &motsnd_voice_ops,
},
{
	.name = "Voice",
	.stream_name = "McBSP3-Codec",
	.cpu_dai = &omap_mcbsp_dai[1],
	.codec_dai = &cpcap_dai[1],
	.init = motsnd_cpcap_voice_init,
	.ops = &motsnd_voice_ops,
},
{
	.name = "VoiceCall",
	.stream_name = "Modem-Codec",
	.cpu_dai = &modem_dai,
	.codec_dai = &cpcap_dai[2],
	.init = motsnd_cpcap_voice_init,
	.ops = &motsnd_incall_ops,
},
{
	.name = "BTCall",
	.stream_name = "Modem-BT",
	.cpu_dai = &modem_dai,
	.codec_dai = &cpcap_dai[3],
	.init = motsnd_cpcap_voice_init,
	.ops = &motsnd_incall_ops,
},
{
	.name = "BTVoice",
	.stream_name = "McBSP3-BT",
	.cpu_dai = &omap_mcbsp_dai[1],
	.codec_dai = &cpcap_dai[4],
	.init = motsnd_cpcap_voice_init,
	.ops = &motsnd_btvoice_ops,
},
};

/* Audio machine driver */
static struct snd_soc_card snd_soc_mot = {
	.name = "motsnd",
	.platform = &omap_soc_platform,
	.dai_link = motsnd_dai,
	.num_links = ARRAY_SIZE(motsnd_dai),
};

/* Audio subsystem */
static struct snd_soc_device mot_snd_devdata = {
	.card = &snd_soc_mot,
	.codec_dev = &soc_codec_dev_cpcap,
};


static struct platform_device *mot_snd_device;

static int __init motsnd_soc_init(void)
{
	int ret;

	pr_info("ENTER: MOTSND SoC init\n");
	mot_snd_device = platform_device_alloc("soc-audio", -1);
	if (!mot_snd_device) {
		printk(KERN_ERR "Platform device allocation failed\n");
		return -ENOMEM;
	}
	snd_soc_register_dai(&modem_dai);
	platform_set_drvdata(mot_snd_device, &mot_snd_devdata);
	mot_snd_devdata.dev = &mot_snd_device->dev;
	*(unsigned int *)motsnd_dai[0].cpu_dai->private_data = 1; /* McBSP2 */
	*(unsigned int *)motsnd_dai[1].cpu_dai->private_data = 2; /* McBSP3 */
	ret = platform_device_add(mot_snd_device);
	if (ret)
		goto err;

	return 0;

err:
	printk(KERN_ERR "Unable to add platform device\n");
	platform_device_put(mot_snd_device);
	return ret;
}
module_init(motsnd_soc_init);

static void __exit motsnd_soc_exit(void)
{
	platform_device_unregister(mot_snd_device);
}
module_exit(motsnd_soc_exit);

MODULE_AUTHOR("Motorola");
MODULE_DESCRIPTION("ALSA SoC MOTSND");
MODULE_LICENSE("GPL");
