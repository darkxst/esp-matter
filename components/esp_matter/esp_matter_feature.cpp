// Copyright 2021 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <esp_log.h>
#include <esp_matter.h>
#include <esp_matter_feature.h>

#include <app-common/zap-generated/cluster-enums.h>

static const char *TAG = "esp_matter_feature";

using namespace chip::app::Clusters;

namespace esp_matter {
namespace cluster {

static esp_err_t update_feature_map(cluster_t *cluster, uint32_t value)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }

    /* Get the attribute */
    attribute_t *attribute = attribute::get(cluster, Globals::Attributes::FeatureMap::Id);

    /* Create the attribute with the new value if it does not exist */
    if (!attribute) {
        attribute = global::attribute::create_feature_map(cluster, value);
        if (!attribute) {
            ESP_LOGE(TAG, "Could not create feature map attribute");
            return ESP_FAIL;
        }
        return ESP_OK;
    }

    /* Update the value if the attribute already exists */
    esp_matter_attr_val_t val = esp_matter_invalid(NULL);
    attribute::get_val(attribute, &val);
    val.val.u32 |= value;
    /* Here we can't call attribute::update() since the chip stack would not have started yet, since we are
    still creating the data model. So, we are directly using attribute::set_val(). */
    return attribute::set_val(attribute, &val);
}

static uint32_t get_feature_map_value(cluster_t *cluster)
{
    attribute_t *attribute = attribute::get(cluster, Globals::Attributes::FeatureMap::Id);

    esp_matter_attr_val_t val = esp_matter_invalid(NULL);
    attribute::get_val(attribute, &val);
    return val.val.u32;
}

namespace on_off {
namespace feature {
namespace lighting {

uint32_t get_id()
{
    return (uint32_t)OnOff::OnOffFeature::kLighting;
}

esp_err_t add(cluster_t *cluster, config_t *config)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());

    /* Attributes not managed internally */
    attribute::create_global_scene_control(cluster, config->global_scene_control);
    attribute::create_on_time(cluster, config->on_time);
    attribute::create_off_wait_time(cluster, config->off_wait_time);
    attribute::create_start_up_on_off(cluster, config->start_up_on_off);

    /* Commands */
    command::create_off_with_effect(cluster);
    command::create_on_with_recall_global_scene(cluster);
    command::create_on_with_timed_off(cluster);

    return ESP_OK;
}

} /* lighting */
} /* feature */
} /* on_off */

namespace level_control {
namespace feature {
namespace on_off {

uint32_t get_id()
{
    return (uint32_t)LevelControl::LevelControlFeature::kOnOff;
}

esp_err_t add(cluster_t *cluster)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());

    return ESP_OK;
}

} /* on_off */

namespace lighting {

uint32_t get_id()
{
    return (uint32_t)LevelControl::LevelControlFeature::kLighting;
}

esp_err_t add(cluster_t *cluster, config_t *config)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());

    /* Attributes not managed internally */
    attribute::create_remaining_time(cluster, config->remaining_time);
    attribute::create_min_level(cluster, config->min_level);
    attribute::create_max_level(cluster, config->max_level);
    attribute::create_start_up_current_level(cluster, config->start_up_current_level);

    return ESP_OK;
}

} /* lighting */

namespace frequency {

uint32_t get_id()
{
    return (uint32_t)LevelControl::LevelControlFeature::kFrequency;
}

esp_err_t add(cluster_t *cluster, config_t *config)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());

    /* Attributes not managed internally */
    attribute::create_current_frequency(cluster, config->current_frequency);
    attribute::create_min_frequency(cluster, config->min_frequency);
    attribute::create_max_frequency(cluster, config->max_frequency);

    /* Commands */
    command::create_move_to_closest_frequency(cluster);

    return ESP_OK;
}
} /* frequency */
} /* feature */
} /* level_control */

namespace color_control {
namespace feature {
namespace hue_saturation {

uint32_t get_id()
{
    return (uint32_t)ColorControl::ColorCapabilities::kHueSaturationSupported;
}

esp_err_t add(cluster_t *cluster, config_t *config)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());

    /* Attributes not managed internally */
    attribute::create_current_hue(cluster, config->current_hue);
    attribute::create_current_saturation(cluster, config->current_saturation);

    /* Commands */
    command::create_move_to_hue(cluster);
    command::create_move_hue(cluster);
    command::create_step_hue(cluster);
    command::create_move_to_saturation(cluster);
    command::create_move_saturation(cluster);
    command::create_step_saturation(cluster);
    command::create_move_to_hue_and_saturation(cluster);
    command::create_stop_move_step(cluster);

    return ESP_OK;
}

} /* hue_saturation */

namespace color_temperature {

uint32_t get_id()
{
    return (uint32_t)ColorControl::ColorCapabilities::kColorTemperatureSupported;
}

esp_err_t add(cluster_t *cluster, config_t *config)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());

    /* Attributes not managed internally */
    attribute::create_color_temperature_mireds(cluster, config->color_temperature_mireds);
    attribute::create_color_temp_physical_min_mireds(cluster, config->color_temp_physical_min_mireds);
    attribute::create_color_temp_physical_max_mireds(cluster, config->color_temp_physical_max_mireds);
    attribute::create_couple_color_temp_to_level_min_mireds(cluster, config->couple_color_temp_to_level_min_mireds);
    attribute::create_startup_color_temperature_mireds(cluster, config->startup_color_temperature_mireds);

    /* Commands */
    command::create_move_to_color_temperature(cluster);
    command::create_stop_move_step(cluster);
    command::create_move_color_temperature(cluster);
    command::create_step_color_temperature(cluster);

    return ESP_OK;
}

} /* color_temperature */

namespace xy {

uint32_t get_id()
{
    return (uint32_t)ColorControl::ColorCapabilities::kXYAttributesSupported;
}

esp_err_t add(cluster_t *cluster, config_t *config)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());

    /* Attributes not managed internally */
    attribute::create_current_x(cluster, config->current_x);
    attribute::create_current_y(cluster, config->current_y);

    /* Commands */
    command::create_move_to_color(cluster);
    command::create_stop_move_step(cluster);
    command::create_move_color(cluster);
    command::create_step_color(cluster);

    return ESP_OK;
}

} /* xy */

namespace enhanced_hue {

uint32_t get_id()
{
    return (uint32_t)chip::app::Clusters::ColorControl::ColorCapabilities::kEnhancedHueSupported;
}

esp_err_t add(cluster_t *cluster, config_t *config)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());

    /* Attributes not managed internally */
    attribute::create_enhanced_current_hue(cluster, config->enhanced_current_hue);

    /* Commands */
    command::create_enhanced_move_to_hue(cluster);
    command::create_enhanced_move_hue(cluster);
    command::create_enhanced_step_hue(cluster);
    command::create_enhanced_move_to_hue_and_saturation(cluster);

    return ESP_OK;
}
} /* enhanced_hue */

namespace color_loop {
uint32_t get_id()
{
    return (uint32_t)chip::app::Clusters::ColorControl::ColorCapabilities::kColorLoopSupported;
}

esp_err_t add(cluster_t *cluster, config_t *config)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());

    /* Attributes not managed internally */
    attribute::create_color_loop_active(cluster, config->color_loop_active);
    attribute::create_color_loop_direction(cluster, config->color_loop_direction);
    attribute::create_color_loop_time(cluster, config->color_loop_time);
    attribute::create_color_loop_start_enhanced_hue(cluster, config->color_loop_start_enhanced_hue);
    attribute::create_color_loop_stored_enhanced_hue(cluster, config->color_loop_stored_enhanced_hue);

    /* Commands */
    command::create_color_loop_set(cluster);

    return ESP_OK;
}
} /* color_loop */
} /* feature */
} /* color_control */

namespace window_covering {
namespace feature {
namespace lift {

uint32_t get_id()
{
    return (uint32_t)WindowCovering::Feature::kLift;
}

esp_err_t add(cluster_t *cluster, config_t *config)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());

    attribute::create_number_of_actuations_lift(cluster, config->number_of_actuations_lift);

    return ESP_OK;
}

} /* lift */

namespace tilt {

uint32_t get_id()
{
    return (uint32_t)WindowCovering::Feature::kTilt;
}

esp_err_t add(cluster_t *cluster, config_t *config)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());

    attribute::create_number_of_actuations_tilt(cluster, config->number_of_actuations_tilt);

    return ESP_OK;
}

} /* tilt */

namespace position_aware_lift {

uint32_t get_id()
{
    return (uint32_t)WindowCovering::Feature::kPositionAwareLift;
}

esp_err_t add(cluster_t *cluster, config_t *config)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());
    uint32_t pa_lt_and_lift_feature_map = get_id() | feature::lift::get_id();
    if((get_feature_map_value(cluster) & pa_lt_and_lift_feature_map) == pa_lt_and_lift_feature_map)
    {
	attribute::create_current_position_lift_percentage(cluster, config->current_position_lift_percentage);
	attribute::create_target_position_lift_percent_100ths(cluster, config->target_position_lift_percent_100ths);
	attribute::create_current_position_lift_percent_100ths(cluster, config->current_position_lift_percent_100ths);

	command::create_go_to_lift_percentage(cluster);
    }else{
	ESP_LOGE(TAG, "Cluster shall support Lift feature");
	return ESP_ERR_NOT_SUPPORTED;
    }
    return ESP_OK;
}
} /* position_aware_lift */

namespace absolute_position {

uint32_t get_id()
{
    return (uint32_t)WindowCovering::Feature::kAbsolutePosition;
}

esp_err_t add(cluster_t *cluster, config_t *config)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());
    uint32_t abs_and_pa_lf_feature_map = get_id() | feature::position_aware_lift::get_id();
    uint32_t abs_and_pa_tl_feature_map = get_id() | feature::position_aware_tilt::get_id();
    uint32_t abs_and_lift_feature_map = get_id() | feature::lift::get_id();
    uint32_t abs_and_tilt_feature_map = get_id() | feature::tilt::get_id();
    if((get_feature_map_value(cluster) & abs_and_pa_lf_feature_map) == abs_and_pa_lf_feature_map)
    {
	attribute::create_physical_closed_limit_lift(cluster, config->physical_closed_limit_lift);
	attribute::create_current_position_lift(cluster, config->current_position_lift);
	attribute::create_installed_open_limit_lift(cluster, config->installed_open_limit_lift);
	attribute::create_installed_closed_limit_lift(cluster, config->installed_open_limit_lift);

    }else{
	ESP_LOGE(TAG, "Cluster shall support Position_Aware_Lift feature");
	return ESP_ERR_NOT_SUPPORTED;
    }

    if((get_feature_map_value(cluster) & abs_and_pa_tl_feature_map) == abs_and_pa_tl_feature_map)
    {
	attribute::create_physical_closed_limit_tilt(cluster, config->physical_closed_limit_tilt);
	attribute::create_current_position_tilt(cluster, config->current_position_tilt);
	attribute::create_installed_open_limit_tilt(cluster, config->installed_open_limit_tilt);
	attribute::create_installed_closed_limit_tilt(cluster, config->installed_open_limit_tilt);

    }else{
	ESP_LOGE(TAG, "Cluster shall support Position_Aware_Tilt feature");
	return ESP_ERR_NOT_SUPPORTED;
    }

    if((get_feature_map_value(cluster) & abs_and_lift_feature_map) == abs_and_lift_feature_map)
    {
	command::create_go_to_lift_value(cluster);
    }

    if((get_feature_map_value(cluster) & abs_and_tilt_feature_map) == abs_and_tilt_feature_map)
    {
	command::create_go_to_tilt_value(cluster);
    }
    return ESP_OK;
}

} /* absolute_position */

namespace position_aware_tilt {

uint32_t get_id()
{
    return (uint32_t)WindowCovering::Feature::kPositionAwareTilt;
}

esp_err_t add(cluster_t *cluster, config_t *config)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());

    uint32_t pa_lt_and_tilt_feature_map = get_id() | feature::tilt::get_id();
    if((get_feature_map_value(cluster) & pa_lt_and_tilt_feature_map) == pa_lt_and_tilt_feature_map)
    {
	attribute::create_current_position_tilt_percentage(cluster, config->current_position_tilt_percentage);
	attribute::create_target_position_tilt_percent_100ths(cluster, config->target_position_tilt_percent_100ths);
	attribute::create_current_position_tilt_percent_100ths(cluster, config->current_position_tilt_percent_100ths);

	command::create_go_to_tilt_percentage(cluster);
    }else{
	ESP_LOGE(TAG, "Cluster shall support Tilt feature");
	return ESP_ERR_NOT_SUPPORTED;
    }
    return ESP_OK;
}

} /* position_aware_tilt */
} /* feature */
} /* window_covering */

namespace diagnostics_network_wifi {
namespace feature {

namespace packets_counts {

uint32_t get_id()
{
    // The WiFiNetworkDiagnosticsFeature enum class is not added in the upstream code.
    // Return the code according to the SPEC
    return 0x01;
}

esp_err_t add(cluster_t *cluster)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());

    /* Attributes managed internally */
    attribute::create_beacon_rx_count(cluster, 0);
    attribute::create_packet_multicast_rx_count(cluster, 0);
    attribute::create_packet_multicast_tx_count(cluster, 0);
    attribute::create_packet_unicast_rx_count(cluster, 0);
    attribute::create_packet_unicast_tx_count(cluster, 0);

    return ESP_OK;
}

} /* packets_counts */

namespace error_counts {

uint32_t get_id()
{
    // The WiFiNetworkDiagnosticsFeature enum class is not added in the upstream code.
    // Return the code according to the SPEC
    return 0x02;
}

esp_err_t add(cluster_t *cluster)
{
    if (!cluster) {
        ESP_LOGE(TAG, "Cluster cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    update_feature_map(cluster, get_id());

    /* Attributes managed internally */
    attribute::create_beacon_lost_count(cluster, 0);
    attribute::create_overrun_count(cluster, 0);

    /* Commands */
    command::create_reset_counts(cluster);

    return ESP_OK;
}

} /* error_counts */

} /* feature */
} /* diagnostics_network_wifi */

} /* cluster */
} /* esp_matter */
