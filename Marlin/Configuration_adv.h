/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#define CONFIG_EXAMPLES_DIR "Creality/Ender-3/CrealityV422"

/**
 * Configuration_adv.h
 *
 * Advanced settings.
 * Only change these if you know exactly what you're doing.
 * Some of these settings can damage your printer if improperly set!
 *
 * Basic settings can be found in Configuration.h
 */
#define CONFIGURATION_ADV_H_VERSION 02010300

// @section develop

/**
 * Configuration Export
 *
 * Export the configuration as part of the build. (See signature.py)
 * Output files are saved with the build (e.g., .pio/build/mega2560).
 *
 * See `build_all_examples --ini` as an example of config.ini archiving.
 *
 *  1 = marlin_config.json - Dictionary containing the configuration.
 *      This file is also generated for CONFIGURATION_EMBEDDING.
 *  2 = config.ini - File format for PlatformIO preprocessing.
 *  3 = schema.json - The entire configuration schema. (13 = pattern groups)
 *  4 = schema.yml - The entire configuration schema.
 *  5 = Config.h - Minimal configuration by popular demand.
 */
//#define CONFIG_EXPORT 105 // :[1:'JSON', 2:'config.ini', 3:'schema.json', 4:'schema.yml', 5:'Config.h']

//===========================================================================
//============================= Thermal Settings ============================
//===========================================================================
// @section temperature

/**
 * Thermocouple sensors are quite sensitive to noise.  Any noise induced in
 * the sensor wires, such as by stepper motor wires run in parallel to them,
 * may result in the thermocouple sensor reporting spurious errors.  This
 * value is the number of errors which can occur in a row before the error
 * is reported.  This allows us to ignore intermittent error conditions while
 * still detecting an actual failure, which should result in a continuous
 * stream of errors from the sensor.
 *
 * Set this value to 0 to fail on the first error to occur.
 */
#define THERMOCOUPLE_MAX_ERRORS 15

//
// M3 screw-in NTC 100 3950
//
#if TEMP_SENSOR_0 == 1000
  #define HOTEND0_PULLUP_RESISTOR_OHMS    10000 // Pullup resistor
  #define HOTEND0_RESISTANCE_25C_OHMS     110000 // Resistance at 25C
  #define HOTEND0_BETA                    3950 // Beta value
  #define HOTEND0_SH_C_COEFF              0.000000172897938 // Steinhart-Hart C coefficient
#endif

//
// Heated Bed Bang-Bang options
//
#if DISABLED(PIDTEMPBED)
  #define BED_CHECK_INTERVAL 5000   // (ms) Interval between checks in bang-bang control
  #if ANY(BED_LIMIT_SWITCHING, PELTIER_BED)
    #define BED_HYSTERESIS 2        // (°C) Only set the relevant heater state when ABS(T-target) > BED_HYSTERESIS
  #endif
#endif


//
// Motherboard Sensor options
//
#if TEMP_SENSOR_BOARD
  #define THERMAL_PROTECTION_BOARD   // Halt the printer if the board sensor leaves the temp range below.
  #define BOARD_MINTEMP           8  // (°C)
  #define BOARD_MAXTEMP          70  // (°C)
  //#define TEMP_BOARD_PIN -1        // Board temp sensor pin override.
#endif

//
// SoC Sensor options
//
#if TEMP_SENSOR_SOC
  #define THERMAL_PROTECTION_SOC     // Halt the printer if the SoC sensor leaves the temp range below.
  #define SOC_MAXTEMP            85  // (°C)
#endif

/**
 * Thermal Protection provides additional protection to your printer from damage
 * and fire. Marlin always includes safe min and max temperature ranges which
 * protect against a broken or disconnected thermistor wire.
 *
 * The issue: If a thermistor falls out, it will report the much lower
 * temperature of the air in the room, and the the firmware will keep
 * the heater on.
 *
 * The solution: Once the temperature reaches the target, start observing.
 * If the temperature stays too far below the target (hysteresis) for too
 * long (period), the firmware will halt the machine as a safety precaution.
 *
 * If you get false positives for "Thermal Runaway", increase
 * THERMAL_PROTECTION_HYSTERESIS and/or THERMAL_PROTECTION_PERIOD
 */
#if ALL(HAS_HOTEND, THERMAL_PROTECTION_HOTENDS)
  #define THERMAL_PROTECTION_PERIOD        40 // (seconds)
  #define THERMAL_PROTECTION_HYSTERESIS     4 // (°C)

  #define ADAPTIVE_FAN_SLOWING              // Slow down the part-cooling fan if the temperature drops
  #if ENABLED(ADAPTIVE_FAN_SLOWING)
    #define REPORT_ADAPTIVE_FAN_SLOWING     // Report fan slowing activity to the console
    #if ANY(MPCTEMP, PIDTEMP)
      #define TEMP_TUNING_MAINTAIN_FAN      // Don't slow down the fan speed during M303 or M306 T
    #endif
  #endif

  /**
   * Whenever an M104, M109, or M303 increases the target temperature, the
   * firmware will wait for the WATCH_TEMP_PERIOD to expire. If the temperature
   * hasn't increased by WATCH_TEMP_INCREASE degrees, the machine is halted and
   * requires a hard reset. This test restarts with any M104/M109/M303, but only
   * if the current temperature is far enough below the target for a reliable
   * test.
   *
   * If you get false positives for "Heating failed", increase WATCH_TEMP_PERIOD
   * and/or decrease WATCH_TEMP_INCREASE. WATCH_TEMP_INCREASE should not be set
   * below 2.
   */
  #define WATCH_TEMP_PERIOD  40               // (seconds)
  #define WATCH_TEMP_INCREASE 2               // (°C)
#endif

/**
 * Thermal Protection parameters for the bed are just as above for hotends.
 */
#if TEMP_SENSOR_BED && ENABLED(THERMAL_PROTECTION_BED)
  #define THERMAL_PROTECTION_BED_PERIOD        20 // (seconds)
  #define THERMAL_PROTECTION_BED_HYSTERESIS     2 // (°C)

  /**
   * As described above, except for the bed (M140/M190/M303).
   */
  #define WATCH_BED_TEMP_PERIOD                60 // (seconds)
  #define WATCH_BED_TEMP_INCREASE               2 // (°C)
#endif


#if ANY(THERMAL_PROTECTION_HOTENDS, THERMAL_PROTECTION_BED, THERMAL_PROTECTION_CHAMBER, THERMAL_PROTECTION_COOLER)
  /**
   * Thermal Protection Variance Monitor - EXPERIMENTAL
   * Kill the machine on a stuck temperature sensor.
   *
   * This feature may cause some thermally-stable systems to halt. Be sure to test it thoroughly under
   * a variety of conditions. Disable if you get false positives.
   *
   * This feature ensures that temperature sensors are updating regularly. If sensors die or get "stuck",
   * or if Marlin stops reading them, temperatures will remain constant while heaters may still be powered!
   * This feature only monitors temperature changes so it should catch any issue, hardware or software.
   *
   * By default it uses the THERMAL_PROTECTION_*_PERIOD constants (above) for the time window, within which
   * at least one temperature change must occur, to indicate that sensor polling is working. If any monitored
   * heater's temperature remains totally constant (without even a fractional change) during this period, a
   * thermal malfunction error occurs and the printer is halted.
   *
   * A very stable heater might produce a false positive and halt the printer. In this case, try increasing
   * the corresponding THERMAL_PROTECTION_*_PERIOD constant a bit. Keep in mind that uncontrolled heating
   * shouldn't be allowed to persist for more than a minute or two.
   *
   * Be careful to distinguish false positives from real sensor issues before disabling this feature. If the
   * heater's temperature appears even slightly higher than expected after restarting, you may have a real
   * thermal malfunction. Check the temperature graph in your host for any unusual bumps.
   */
  #define THERMAL_PROTECTION_VARIANCE_MONITOR
  #if ENABLED(THERMAL_PROTECTION_VARIANCE_MONITOR)
    // Variance detection window to override the THERMAL_PROTECTION...PERIOD settings above.
    // Keep in mind that some heaters heat up faster than others.
    //#define THERMAL_PROTECTION_VARIANCE_MONITOR_PERIOD 30  // (s) Override all watch periods
  #endif
#endif

#if ENABLED(PIDTEMP)
  // Add an additional term to the heater power, proportional to the extrusion speed.
  // A well-chosen Kc value should add just enough power to melt the increased material volume.
  // #define PID_EXTRUSION_SCALING
  #if ENABLED(PID_EXTRUSION_SCALING)
    #define DEFAULT_Kc (100) // heating power = Kc * e_speed
    #define LPQ_MAX_LEN 50
  #endif

  /**
   * Add an additional term to the heater power, proportional to the fan speed.
   * A well-chosen Kf value should add just enough power to compensate for power-loss from the cooling fan.
   * You can either just add a constant compensation with the DEFAULT_Kf value
   * or follow the instruction below to get speed-dependent compensation.
   *
   * Constant compensation (use only with fan speeds of 0% and 100%)
   * ---------------------------------------------------------------------
   * A good starting point for the Kf-value comes from the calculation:
   *   kf = (power_fan * eff_fan) / power_heater * 255
   * where eff_fan is between 0.0 and 1.0, based on fan-efficiency and airflow to the nozzle / heater.
   *
   * Example:
   *   Heater: 40W, Fan: 0.1A * 24V = 2.4W, eff_fan = 0.8
   *   Kf = (2.4W * 0.8) / 40W * 255 = 12.24
   *
   * Fan-speed dependent compensation
   * --------------------------------
   * 1. To find a good Kf value, set the hotend temperature, wait for it to settle, and enable the fan (100%).
   *    Make sure PID_FAN_SCALING_LIN_FACTOR is 0 and PID_FAN_SCALING_ALTERNATIVE_DEFINITION is not enabled.
   *    If you see the temperature drop repeat the test, increasing the Kf value slowly, until the temperature
   *    drop goes away. If the temperature overshoots after enabling the fan, the Kf value is too big.
   * 2. Note the Kf-value for fan-speed at 100%
   * 3. Determine a good value for PID_FAN_SCALING_MIN_SPEED, which is around the speed, where the fan starts moving.
   * 4. Repeat step 1. and 2. for this fan speed.
   * 5. Enable PID_FAN_SCALING_ALTERNATIVE_DEFINITION and enter the two identified Kf-values in
   *    PID_FAN_SCALING_AT_FULL_SPEED and PID_FAN_SCALING_AT_MIN_SPEED. Enter the minimum speed in PID_FAN_SCALING_MIN_SPEED
   */
  //#define PID_FAN_SCALING
  #if ENABLED(PID_FAN_SCALING)
    //#define PID_FAN_SCALING_ALTERNATIVE_DEFINITION
    #if ENABLED(PID_FAN_SCALING_ALTERNATIVE_DEFINITION)
      // The alternative definition is used for an easier configuration.
      // Just figure out Kf at full speed (255) and PID_FAN_SCALING_MIN_SPEED.
      // DEFAULT_Kf and PID_FAN_SCALING_LIN_FACTOR are calculated accordingly.

      #define PID_FAN_SCALING_AT_FULL_SPEED 13.0        //=PID_FAN_SCALING_LIN_FACTOR*255+DEFAULT_Kf
      #define PID_FAN_SCALING_AT_MIN_SPEED   6.0        //=PID_FAN_SCALING_LIN_FACTOR*PID_FAN_SCALING_MIN_SPEED+DEFAULT_Kf
      #define PID_FAN_SCALING_MIN_SPEED     10.0        // Minimum fan speed at which to enable PID_FAN_SCALING

      #define DEFAULT_Kf (255.0*PID_FAN_SCALING_AT_MIN_SPEED-PID_FAN_SCALING_AT_FULL_SPEED*PID_FAN_SCALING_MIN_SPEED)/(255.0-PID_FAN_SCALING_MIN_SPEED)
      #define PID_FAN_SCALING_LIN_FACTOR (PID_FAN_SCALING_AT_FULL_SPEED-DEFAULT_Kf)/255.0

    #else
      #define PID_FAN_SCALING_LIN_FACTOR (0)             // Power-loss due to cooling = Kf * (fan_speed)
      #define DEFAULT_Kf 10                              // A constant value added to the PID-tuner
      #define PID_FAN_SCALING_MIN_SPEED 10               // Minimum fan speed at which to enable PID_FAN_SCALING
    #endif
  #endif
#endif

/**
 * Automatic Temperature Mode
 *
 * Dynamically adjust the hotend target temperature based on planned E moves.
 *
 * (Contrast with PID_EXTRUSION_SCALING, which tracks E movement and adjusts PID
 *  behavior using an additional kC value.)
 *
 * Autotemp is calculated by (mintemp + factor * mm_per_sec), capped to maxtemp.
 *
 * Enable Autotemp Mode with M104/M109 F<factor> S<mintemp> B<maxtemp>.
 * Disable by sending M104/M109 with no F parameter (or F0 with AUTOTEMP_PROPORTIONAL).
 */
#define AUTOTEMP
#if ENABLED(AUTOTEMP)
  #define AUTOTEMP_OLDWEIGHT    0.98  // Factor used to weight previous readings (0.0 < value < 1.0)
  #define AUTOTEMP_MIN          200
  #define AUTOTEMP_MAX          260
  #define AUTOTEMP_FACTOR       0.1f
  // Turn on AUTOTEMP on M104/M109 by default using proportions set here
  #define AUTOTEMP_PROPORTIONAL
  #if ENABLED(AUTOTEMP_PROPORTIONAL)
    #define AUTOTEMP_MIN_P      0 // (°C) Added to the target temperature
    #define AUTOTEMP_MAX_P      5 // (°C) Added to the target temperature
    #define AUTOTEMP_FACTOR_P   1 // Apply this F parameter by default (overridden by M104/M109 F)
  #endif
#endif

// Show Temperature ADC value
// Enable for M105 to include ADC values read from temperature sensors.
#define SHOW_TEMP_ADC_VALUES

/**
 * High Temperature Thermistor Support
 *
 * Thermistors able to support high temperature tend to have a hard time getting
 * good readings at room and lower temperatures. This means TEMP_SENSOR_X_RAW_LO_TEMP
 * will probably be caught when the heating element first turns on during the
 * preheating process, which will trigger a MINTEMP error as a safety measure
 * and force stop everything.
 * To circumvent this limitation, we allow for a preheat time (during which,
 * MINTEMP error won't be triggered) and add a min_temp buffer to handle
 * aberrant readings.
 *
 * If you want to enable this feature for your hotend thermistor(s)
 * uncomment and set values > 0 in the constants below
 */

// The number of consecutive low temperature errors that can occur
// before a MINTEMP error is triggered. (Shouldn't be more than 10.)
//#define MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED 0

/**
 * The number of milliseconds a hotend will preheat before starting to check
 * the temperature. This value should NOT be set to the time it takes the
 * hot end to reach the target temperature, but the time it takes to reach
 * the minimum temperature your thermistor can read. The lower the better/safer.
 * This shouldn't need to be more than 30 seconds (30000)
 */
//#define PREHEAT_TIME_HOTEND_MS 0
//#define PREHEAT_TIME_BED_MS 0

// @section extruder

/**
 * Extruder runout prevention.
 * If the machine is idle and the temperature over MINTEMP
 * then extrude some filament every couple of SECONDS.
 */
//#define EXTRUDER_RUNOUT_PREVENT
#if ENABLED(EXTRUDER_RUNOUT_PREVENT)
  #define EXTRUDER_RUNOUT_MINTEMP 190
  #define EXTRUDER_RUNOUT_SECONDS 30
  #define EXTRUDER_RUNOUT_SPEED 1500  // (mm/min)
  #define EXTRUDER_RUNOUT_EXTRUDE 5   // (mm)
#endif

/**
 * Hotend Idle Timeout
 * Prevent filament in the nozzle from charring and causing a critical jam.
 */
//#define HOTEND_IDLE_TIMEOUT
#if ENABLED(HOTEND_IDLE_TIMEOUT)
  #define HOTEND_IDLE_TIMEOUT_SEC (5*60)    // (seconds) Time without extruder movement to trigger protection
  #define HOTEND_IDLE_MIN_TRIGGER   180     // (°C) Minimum temperature to enable hotend protection
  #define HOTEND_IDLE_NOZZLE_TARGET   0     // (°C) Safe temperature for the nozzle after timeout
  #define HOTEND_IDLE_BED_TARGET      0     // (°C) Safe temperature for the bed after timeout
#endif

// @section temperature

// Calibration for AD595 / AD8495 sensor to adjust temperature measurements.
// The final temperature is calculated as (measuredTemp * GAIN) + OFFSET.
#define TEMP_SENSOR_AD595_OFFSET  0.0
#define TEMP_SENSOR_AD595_GAIN    1.0
#define TEMP_SENSOR_AD8495_OFFSET 0.0
#define TEMP_SENSOR_AD8495_GAIN   1.0

// @section fans

/**
 * Controller Fan
 * To cool down the stepper drivers and MOSFETs.
 *
 * The fan turns on automatically whenever any driver is enabled and turns
 * off (or reduces to idle speed) shortly after drivers are turned off.
 */
// @todo look at current double fan config
// #define USE_CONTROLLER_FAN
#if ENABLED(USE_CONTROLLER_FAN)
  //#define CONTROLLER_FAN_PIN -1           // Set a custom pin for the controller fan
  //#define CONTROLLER_FAN2_PIN -1          // Set a custom pin for second controller fan
  //#define CONTROLLER_FAN_USE_Z_ONLY       // With this option only the Z axis is considered
  //#define CONTROLLER_FAN_IGNORE_Z         // Ignore Z stepper. Useful when stepper timeout is disabled.
  #define CONTROLLERFAN_SPEED_MIN         0 // (0-255) Minimum speed. (If set below this value the fan is turned off.)
  #define CONTROLLERFAN_SPEED_ACTIVE    255 // (0-255) Active speed, used when any motor is enabled
  #define CONTROLLERFAN_SPEED_IDLE       40 // (0-255) Idle speed, used when motors are disabled
  #define CONTROLLERFAN_IDLE_TIME        60 // (seconds) Extra time to keep the fan running after disabling motors

  // Use TEMP_SENSOR_BOARD as a trigger for enabling the controller fan
  //#define CONTROLLER_FAN_MIN_BOARD_TEMP 40  // (°C) Turn on the fan if the board reaches this temperature

  // Use TEMP_SENSOR_SOC as a trigger for enabling the controller fan
  //#define CONTROLLER_FAN_MIN_SOC_TEMP 40  // (°C) Turn on the fan if the SoC reaches this temperature

  #define CONTROLLER_FAN_BED_HEATING        // Turn on the fan when heating the bed

  //#define CONTROLLER_FAN_EDITABLE         // Enable M710 configurable settings
  #if ENABLED(CONTROLLER_FAN_EDITABLE)
    #define CONTROLLER_FAN_MENU             // Enable the Controller Fan submenu
  #endif
#endif

/**
 * Fan Kickstart
 * When part cooling or controller fans first start, run at a speed that
 * gets it spinning reliably for a short time before setting the requested speed.
 * (Does not work on Sanguinololu with FAN_SOFT_PWM.)
 */
//#define FAN_KICKSTART_TIME  100  // (ms)
//#define FAN_KICKSTART_POWER 180  // 64-255
//#define FAN_KICKSTART_LINEAR     // Set kickstart time linearly based on the speed, e.g., for 20% (51) it will be FAN_KICKSTART_TIME * 0.2.
                                   // Useful for quick speed up to low speed. Kickstart power must be set to 255.

// Some coolers may require a non-zero "off" state.
//#define FAN_OFF_PWM  1

/**
 * PWM Fan Scaling
 *
 * Define the min/max speeds for PWM fans (as set with M106).
 *
 * With these options the M106 0-255 value range is scaled to a subset
 * to ensure that the fan has enough power to spin, or to run lower
 * current fans with higher current. (e.g., 5V/12V fans with 12V/24V)
 * Value 0 always turns off the fan.
 *
 * Define one or both of these to override the default 0-255 range.
 */
//#define FAN_MIN_PWM 50
//#define FAN_MAX_PWM 128

/**
 * Fan Fast PWM
 *
 * Combinations of PWM Modes, prescale values and TOP resolutions are used internally
 * to produce a frequency as close as possible to the desired frequency.
 *
 * FAST_PWM_FAN_FREQUENCY
 *   Set this to your desired frequency.
 *   For AVR, if left undefined this defaults to F = F_CPU/(2*255*1)
 *            i.e., F = 31.4kHz on 16MHz micro-controllers or F = 39.2kHz on 20MHz micro-controllers.
 *   For non AVR, if left undefined this defaults to F = 1Khz.
 *   This F value is only to protect the hardware from an absence of configuration
 *   and not to complete it when users are not aware that the frequency must be specifically set to support the target board.
 *
 *   NOTE: Setting very low frequencies (< 10 Hz) may result in unexpected timer behavior.
 *         Setting very high frequencies can damage your hardware.
 *
 * USE_OCR2A_AS_TOP [undefined by default]
 *   Boards that use TIMER2 for PWM have limitations resulting in only a few possible frequencies on TIMER2:
 *   16MHz MCUs: [62.5kHz, 31.4kHz (default), 7.8kHz, 3.92kHz, 1.95kHz, 977Hz, 488Hz, 244Hz, 60Hz, 122Hz, 30Hz]
 *   20MHz MCUs: [78.1kHz, 39.2kHz (default), 9.77kHz, 4.9kHz, 2.44kHz, 1.22kHz, 610Hz, 305Hz, 153Hz, 76Hz, 38Hz]
 *   A greater range can be achieved by enabling USE_OCR2A_AS_TOP. But note that this option blocks the use of
 *   PWM on pin OC2A. Only use this option if you don't need PWM on 0C2A. (Check your schematic.)
 *   USE_OCR2A_AS_TOP sacrifices duty cycle control resolution to achieve this broader range of frequencies.
 */
//#define FAST_PWM_FAN    // Increase the fan PWM frequency. Removes the PWM noise but increases heating in the FET/Arduino
#if ENABLED(FAST_PWM_FAN)
  //#define FAST_PWM_FAN_FREQUENCY 31400  // Define here to override the defaults below
  //#define USE_OCR2A_AS_TOP
  #ifndef FAST_PWM_FAN_FREQUENCY
    #ifdef __AVR__
      #define FAST_PWM_FAN_FREQUENCY ((F_CPU) / (2 * 255 * 1))
    #else
      #define FAST_PWM_FAN_FREQUENCY 1000U
    #endif
  #endif
#endif

/**
 * Assign more PWM fans for part cooling, synchronized with Fan 0
 */
//#define REDUNDANT_PART_COOLING_FAN 1  // Index of the first fan to synchronize with Fan 0
#ifdef REDUNDANT_PART_COOLING_FAN
  //#define NUM_REDUNDANT_FANS 1        // Number of sequential fans to synchronize with Fan 0
#endif

/**
 * Extruder cooling fans
 *
 * Extruder auto fans automatically turn on when their extruders'
 * temperatures go above EXTRUDER_AUTO_FAN_TEMPERATURE.
 *
 * Your board's pins file specifies the recommended pins. Override those here
 * or set to -1 to disable completely.
 *
 * Multiple extruders can be assigned to the same pin in which case
 * the fan will turn on when any selected extruder is above the threshold.
 */
#define E0_AUTO_FAN_PIN -1
#define COOLER_AUTO_FAN_PIN -1

#define EXTRUDER_AUTO_FAN_TEMPERATURE 50
#define EXTRUDER_AUTO_FAN_SPEED 255   // 255 == full speed
#define COOLER_AUTO_FAN_TEMPERATURE 18
#define COOLER_AUTO_FAN_SPEED 255

/**
 * Hotend Cooling Fans tachometers
 *
 * Define one or more tachometer pins to enable fan speed
 * monitoring, and reporting of fan speeds with M123.
 *
 * NOTE: Only works with fans up to 7000 RPM.
 */
//#define FOURWIRES_FANS      // Needed with AUTO_FAN when 4-wire PWM fans are installed
//#define E0_FAN_TACHO_PIN -1
//#define E0_FAN_TACHO_PULLUP
//#define E0_FAN_TACHO_PULLDOWN

/**
 * Part-Cooling Fan Multiplexer
 *
 * This feature allows you to digitally multiplex the fan output.
 * The multiplexer is automatically switched at tool-change.
 * Set FANMUX[012]_PINs below for up to 2, 4, or 8 multiplexed fans.
 */
#define FANMUX0_PIN -1
#define FANMUX1_PIN -1
#define FANMUX2_PIN -1

// @section endstops

// If you want endstops to stay on (by default) even when not homing
// enable this option. Override at any time with M120, M121.
//#define ENDSTOPS_ALWAYS_ON_DEFAULT

// @section extras

#define Z_LATE_ENABLE // Enable Z the last moment. Needed if your Z driver overheats.


// @section homing

/**
 * Homing Procedure
 * Homing (G28) does an indefinite move towards the endstops to establish
 * the position of the toolhead relative to the workspace.
 */

//#define SENSORLESS_BACKOFF_MM  { 2, 2, 0 }  // (linear=mm, rotational=°) Backoff from endstops before sensorless homing

#define HOMING_BUMP_MM      { 5, 5, 2 }       // (linear=mm, rotational=°) Backoff from endstops after first bump
#define HOMING_BUMP_DIVISOR { 2, 2, 4 }       // Re-Bump Speed Divisor (Divides the Homing Feedrate)

//#define HOMING_BACKOFF_POST_MM { 2, 2, 2 }  // (linear=mm, rotational=°) Backoff from endstops after homing
//#define XY_COUNTERPART_BACKOFF_MM 0         // (mm) Backoff X after homing Y, and vice-versa

#define QUICK_HOME                            // If G28 contains XY do a diagonal move first
//#define HOME_Y_BEFORE_X                     // If G28 contains XY home Y before X
//#define HOME_Z_FIRST                        // Home Z first. Requires a real endstop (not a probe).
//#define CODEPENDENT_XY_HOMING               // If X/Y can't home without homing Y/X first

// @section bltouch

#if ENABLED(BLTOUCH)
  /**
   * Either: Use the defaults (recommended) or: For special purposes, use the following DEFINES
   * Do not activate settings that the probe might not understand. Clones might misunderstand
   * advanced commands.
   *
   * Note: If the probe is not deploying, do a "Reset" and "Self-Test" and then check the
   *       wiring of the BROWN, RED and ORANGE wires.
   *
   * Note: If the trigger signal of your probe is not being recognized, it has been very often
   *       because the BLACK and WHITE wires needed to be swapped. They are not "interchangeable"
   *       like they would be with a real switch. So please check the wiring first.
   *
   * Settings for all BLTouch and clone probes:
   */

  // Safety: The probe needs time to recognize the command.
  //         Minimum command delay (ms). Enable and increase if needed.
  //#define BLTOUCH_DELAY 500

  /**
   * Settings for BLTOUCH Classic 1.2, 1.3 or BLTouch Smart 1.0, 2.0, 2.2, 3.0, 3.1, and most clones:
   */

  // Feature: Switch into SW mode after a deploy. It makes the output pulse longer. Can be useful
  //          in special cases, like noisy or filtered input configurations.
  //#define BLTOUCH_FORCE_SW_MODE

  /**
   * Settings for BLTouch Smart 3.0 and 3.1
   * Summary:
   *   - Voltage modes: 5V and OD (open drain - "logic voltage free") output modes
   *   - High-Speed mode
   *   - Disable LCD voltage options
   */

  /**
   * Danger: Don't activate 5V mode unless attached to a 5V-tolerant controller!
   * V3.0 or 3.1: Set default mode to 5V mode at Marlin startup.
   * If disabled, OD mode is the hard-coded default on 3.0
   * On startup, Marlin will compare its EEPROM to this value. If the selected mode
   * differs, a mode set EEPROM write will be completed at initialization.
   * Use the option below to force an EEPROM write to a V3.1 probe regardless.
   */
  //#define BLTOUCH_SET_5V_MODE

  // Safety: Enable voltage mode settings in the LCD menu.
  //#define BLTOUCH_LCD_VOLTAGE_MENU

  /**
   * Safety: Activate if connecting a probe with an unknown voltage mode.
   * V3.0: Set a probe into mode selected above at Marlin startup. Required for 5V mode on 3.0
   * V3.1: Force a probe with unknown mode into selected mode at Marlin startup ( = Probe EEPROM write )
   * To preserve the life of the probe, use this once then turn it off and re-flash.
   */
  //#define BLTOUCH_FORCE_MODE_SET

  /**
   * Enable "HIGH SPEED" option for probing.
   * Danger: Disable if your probe sometimes fails. Only suitable for stable well-adjusted systems.
   * This feature was designed for Deltabots with very fast Z moves; however, higher speed Cartesians
   * might be able to use it. If the machine can't raise Z fast enough the BLTouch may go into ALARM.
   *
   * Set the default state here, change with 'M401 S' or UI, use M500 to save, M502 to reset.
   */
  // #define BLTOUCH_HS_MODE true

  #ifdef BLTOUCH_HS_MODE
    // The probe Z offset (M851 Z) is the height at which the probe triggers.
    // This must be large enough to keep the probe pin off the bed and prevent
    // it from snagging on the bed clips.
    #define BLTOUCH_HS_EXTRA_CLEARANCE   4 // Extra Z Clearance
  #endif

#endif // BLTOUCH

// @section calibrate

/**
 * Assisted Tramming
 *
 * Add the G35 command to measure bed corners and help adjust screws. Requires a bed probe.
 */
#define ASSISTED_TRAMMING
#if ENABLED(ASSISTED_TRAMMING)

  // Define from 3 to 9 points to probe.
  #define TRAMMING_POINT_XY { { 20, 20 }, { 180,  20 }, { 180, 180 }, { 20, 180 } }

  // Define position names for probe points.
  #define TRAMMING_POINT_NAME_1 "Front-Left"
  #define TRAMMING_POINT_NAME_2 "Front-Right"
  #define TRAMMING_POINT_NAME_3 "Back-Right"
  #define TRAMMING_POINT_NAME_4 "Back-Left"

  #define RESTORE_LEVELING_AFTER_G35    // Enable to restore leveling setup after operation
  #define REPORT_TRAMMING_MM          // Report Z deviation (mm) for each point relative to the first

  #define ASSISTED_TRAMMING_WIZARD    // Add a Tramming Wizard to the LCD menu

  //#define ASSISTED_TRAMMING_WAIT_POSITION { X_CENTER, Y_CENTER, 30 } // Move the nozzle out of the way for adjustment

  /**
   * Screw Thread. Use one of the following defines:
   *
   *   M3_CW = M3 Clockwise, M3_CCW = M3 Counter-Clockwise
   *   M4_CW = M4 Clockwise, M4_CCW = M4 Counter-Clockwise
   *   M5_CW = M5 Clockwise, M5_CCW = M5 Counter-Clockwise
   *
   * :{'M3_CW':'M3 Clockwise','M3_CCW':'M3 Counter-Clockwise','M4_CW':'M4 Clockwise','M4_CCW':'M4 Counter-Clockwise','M5_CW':'M5 Clockwise','M5_CCW':'M5 Counter-Clockwise'}
   */
  #define TRAMMING_SCREW_THREAD M4_CW

#endif

// @section motion control

/**
 * Fixed-time-based Motion Control -- BETA FEATURE
 * Enable/disable and set parameters with G-code M493.
 * See ft_types.h for named values used by FTM options.
 */
//#define FT_MOTION
#if ENABLED(FT_MOTION)
  //#define FTM_IS_DEFAULT_MOTION                 // Use FT Motion as the factory default?
  #define FTM_DEFAULT_DYNFREQ_MODE dynFreqMode_DISABLED // Default mode of dynamic frequency calculation. (DISABLED, Z_BASED, MASS_BASED)
  #define FTM_DEFAULT_SHAPER_X      ftMotionShaper_NONE // Default shaper mode on X axis (NONE, ZV, ZVD, ZVDD, ZVDDD, EI, 2HEI, 3HEI, MZV)
  #define FTM_DEFAULT_SHAPER_Y      ftMotionShaper_NONE // Default shaper mode on Y axis
  #define FTM_SHAPING_DEFAULT_FREQ_X   37.0f      // (Hz) Default peak frequency used by input shapers
  #define FTM_SHAPING_DEFAULT_FREQ_Y   37.0f      // (Hz) Default peak frequency used by input shapers
  #define FTM_LINEAR_ADV_DEFAULT_ENA   false      // Default linear advance enable (true) or disable (false)
  #define FTM_LINEAR_ADV_DEFAULT_K      0.0f      // Default linear advance gain. (Acceleration-based scaling factor.)
  #define FTM_SHAPING_ZETA_X            0.1f      // Zeta used by input shapers for X axis
  #define FTM_SHAPING_ZETA_Y            0.1f      // Zeta used by input shapers for Y axis

  #define FTM_SHAPING_V_TOL_X           0.05f     // Vibration tolerance used by EI input shapers for X axis
  #define FTM_SHAPING_V_TOL_Y           0.05f     // Vibration tolerance used by EI input shapers for Y axis

  //#define FT_MOTION_MENU                        // Provide a MarlinUI menu to set M493 parameters

  /**
   * Advanced configuration
   */
  #define FTM_UNIFIED_BWS                         // DON'T DISABLE unless you use Ulendo FBS (not implemented)
  #if ENABLED(FTM_UNIFIED_BWS)
    #define FTM_BW_SIZE               100         // Unified Window and Batch size with a ratio of 2
  #else
    #define FTM_WINDOW_SIZE           200         // Custom Window size for trajectory generation needed by Ulendo FBS
    #define FTM_BATCH_SIZE            100         // Custom Batch size for trajectory generation needed by Ulendo FBS
  #endif

  #define FTM_FS                     1000         // (Hz) Frequency for trajectory generation. (Reciprocal of FTM_TS)
  #define FTM_TS                        0.001f    // (s) Time step for trajectory generation. (Reciprocal of FTM_FS)

  #if DISABLED(COREXY)
    #define FTM_STEPPER_FS          20000         // (Hz) Frequency for stepper I/O update

    // Use this to adjust the time required to consume the command buffer.
    // Try increasing this value if stepper motion is choppy.
    #define FTM_STEPPERCMD_BUFF_SIZE 3000         // Size of the stepper command buffers

  #else
    // CoreXY motion needs a larger buffer size. These values are based on our testing.
    #define FTM_STEPPER_FS          30000
    #define FTM_STEPPERCMD_BUFF_SIZE 6000
  #endif

  #define FTM_STEPS_PER_UNIT_TIME (FTM_STEPPER_FS / FTM_FS)       // Interpolated stepper commands per unit time
  #define FTM_CTS_COMPARE_VAL (FTM_STEPS_PER_UNIT_TIME / 2)       // Comparison value used in interpolation algorithm
  #define FTM_MIN_TICKS ((STEPPER_TIMER_RATE) / (FTM_STEPPER_FS)) // Minimum stepper ticks between steps

  #define FTM_MIN_SHAPE_FREQ           10         // Minimum shaping frequency
  #define FTM_RATIO (FTM_FS / FTM_MIN_SHAPE_FREQ) // Factor for use in FTM_ZMAX. DON'T CHANGE.
  #define FTM_ZMAX (FTM_RATIO * 2)                // Maximum delays for shaping functions (even numbers only!)
                                                  // Calculate as:
                                                  //   ZV       : FTM_RATIO / 2
                                                  //   ZVD, MZV : FTM_RATIO
                                                  //   2HEI     : FTM_RATIO * 3 / 2
                                                  //   3HEI     : FTM_RATIO * 2
#endif

/**
 * Input Shaping
 *
 * Zero Vibration (ZV) Input Shaping for X and/or Y movements.
 *
 * This option uses a lot of SRAM for the step buffer. The buffer size is
 * calculated automatically from SHAPING_FREQ_[XYZ], DEFAULT_AXIS_STEPS_PER_UNIT,
 * DEFAULT_MAX_FEEDRATE and ADAPTIVE_STEP_SMOOTHING. The default calculation can
 * be overridden by setting SHAPING_MIN_FREQ and/or SHAPING_MAX_FEEDRATE.
 * The higher the frequency and the lower the feedrate, the smaller the buffer.
 * If the buffer is too small at runtime, input shaping will have reduced
 * effectiveness during high speed movements.
 *
 * Tune with M593 D<factor> F<frequency>
 */
//#define INPUT_SHAPING_X
//#define INPUT_SHAPING_Y
//#define INPUT_SHAPING_Z
#if ANY(INPUT_SHAPING_X, INPUT_SHAPING_Y, INPUT_SHAPING_Z)
  #if ENABLED(INPUT_SHAPING_X)
    #define SHAPING_FREQ_X  40.0        // (Hz) The default dominant resonant frequency on the X axis.
    #define SHAPING_ZETA_X   0.15       // Damping ratio of the X axis (range: 0.0 = no damping to 1.0 = critical damping).
  #endif
  #if ENABLED(INPUT_SHAPING_Y)
    #define SHAPING_FREQ_Y  40.0        // (Hz) The default dominant resonant frequency on the Y axis.
    #define SHAPING_ZETA_Y   0.15       // Damping ratio of the Y axis (range: 0.0 = no damping to 1.0 = critical damping).
  #endif
  #if ENABLED(INPUT_SHAPING_Z)
    #define SHAPING_FREQ_Z  40.0        // (Hz) The default dominant resonant frequency on the Z axis.
    #define SHAPING_ZETA_Z   0.15       // Damping ratio of the Z axis (range: 0.0 = no damping to 1.0 = critical damping).
  #endif
  //#define SHAPING_MIN_FREQ  20.0      // (Hz) By default the minimum of the shaping frequencies. Override to affect SRAM usage.
  //#define SHAPING_MAX_STEPRATE 10000  // By default the maximum total step rate of the shaped axes. Override to affect SRAM usage.
  //#define SHAPING_MENU                // Add a menu to the LCD to set shaping parameters.
#endif

// @section motion

#define AXIS_RELATIVE_MODES { false, false, false, false }

// By default stepper drivers require an active-HIGH signal but some high-power drivers require an active-LOW signal to step.
#define STEP_STATE_X HIGH
#define STEP_STATE_Y HIGH
#define STEP_STATE_Z HIGH
#define STEP_STATE_E HIGH

/**
 * Idle Stepper Shutdown
 * Enable DISABLE_IDLE_* to shut down axis steppers after an idle period.
 * The default timeout duration can be overridden with M18 and M84. Set to 0 for No Timeout.
 */
#define DEFAULT_STEPPER_TIMEOUT_SEC 120
#define DISABLE_IDLE_X
#define DISABLE_IDLE_Y
#define DISABLE_IDLE_Z    // Disable if the nozzle could fall onto your printed part!
#define DISABLE_IDLE_E    // Shut down all idle extruders

// Default Minimum Feedrates for printing and travel moves
#define DEFAULT_MINIMUMFEEDRATE       0.0     // (mm/s) Minimum feedrate. Set with M205 S.
#define DEFAULT_MINTRAVELFEEDRATE     0.0     // (mm/s) Minimum travel feedrate. Set with M205 T.

// Minimum time that a segment needs to take as the buffer gets emptied
#define DEFAULT_MINSEGMENTTIME        20000   // (µs) Set with M205 B.

// Slow down the machine if the lookahead buffer is (by default) half full.
// Increase the slowdown divisor for larger buffer sizes.
#define SLOWDOWN
#if ENABLED(SLOWDOWN)
  #define SLOWDOWN_DIVISOR 2
#endif

/**
 * XY Frequency limit
 * Reduce resonance by limiting the frequency of small zigzag infill moves.
 * See https://hydraraptor.blogspot.com/2010/12/frequency-limit.html
 * Use M201 F<freq> S<min%> to change limits at runtime.
 */
//#define XY_FREQUENCY_LIMIT      10 // (Hz) Maximum frequency of small zigzag infill moves. Set with M201 F<hertz>.
#ifdef XY_FREQUENCY_LIMIT
  #define XY_FREQUENCY_MIN_PERCENT 5 // (%) Minimum FR percentage to apply. Set with M201 S<min%>.
#endif

//
// Backlash Compensation
// Adds extra movement to axes on direction-changes to account for backlash.
//
//#define BACKLASH_COMPENSATION
#if ENABLED(BACKLASH_COMPENSATION)
  // Define values for backlash distance and correction.
  // If BACKLASH_GCODE is enabled these values are the defaults.
  #define BACKLASH_DISTANCE_MM { 0, 0, 0 } // (linear=mm, rotational=°) One value for each linear axis
  #define BACKLASH_CORRECTION    0.0       // 0.0 = no correction; 1.0 = full correction

  // Add steps for motor direction changes on CORE kinematics
  //#define CORE_BACKLASH

  // Set BACKLASH_SMOOTHING_MM to spread backlash correction over multiple segments
  // to reduce print artifacts. (Enabling this is costly in memory and computation!)
  //#define BACKLASH_SMOOTHING_MM 3 // (mm)

  // Add runtime configuration and tuning of backlash values (M425)
  //#define BACKLASH_GCODE

  #if ENABLED(BACKLASH_GCODE)
    // Measure the Z backlash when probing (G29) and set with "M425 Z"
    #define MEASURE_BACKLASH_WHEN_PROBING

    #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
      // When measuring, the probe will move up to BACKLASH_MEASUREMENT_LIMIT
      // mm away from point of contact in BACKLASH_MEASUREMENT_RESOLUTION
      // increments while checking for the contact to be broken.
      #define BACKLASH_MEASUREMENT_LIMIT       0.5   // (mm)
      #define BACKLASH_MEASUREMENT_RESOLUTION  0.005 // (mm)
      #define BACKLASH_MEASUREMENT_FEEDRATE    Z_PROBE_FEEDRATE_SLOW // (mm/min)
    #endif
  #endif
#endif


/**
 * Multi-stepping sends steps in bursts to reduce MCU usage for high step-rates.
 * This allows higher feedrates than the MCU could otherwise support.
 */
#define MULTISTEPPING_LIMIT   16  //: [1, 2, 4, 8, 16, 32, 64, 128]

/**
 * Adaptive Step Smoothing increases the resolution of multi-axis moves, particularly at step frequencies
 * below 1kHz (for AVR) or 10kHz (for ARM), where aliasing between axes in multi-axis moves causes audible
 * vibration and surface artifacts. The algorithm adapts to provide the best possible step smoothing at the
 * lowest stepping frequencies.
 */
#define ADAPTIVE_STEP_SMOOTHING

//===========================================================================
//=============================Additional Features===========================
//===========================================================================

// @section lcd

#if HAS_MANUAL_MOVE_MENU
  #define MANUAL_FEEDRATE { 50*60, 50*60, 4*60, 2*60 } // (mm/min) Feedrates for manual moves along X, Y, Z, E from panel
  #define FINE_MANUAL_MOVE 0.025    // (mm) Smallest manual move (< 0.1mm) applying to Z on most machines
#endif

// Change values more rapidly when the encoder is rotated faster
#define ENCODER_RATE_MULTIPLIER
#if ENABLED(ENCODER_RATE_MULTIPLIER)
  #define ENCODER_10X_STEPS_PER_SEC   30  // (steps/s) Encoder rate for 10x speed
  #define ENCODER_100X_STEPS_PER_SEC  80  // (steps/s) Encoder rate for 100x speed
#endif

// Play a beep when the feedrate is changed from the Status Screen
#define BEEP_ON_FEEDRATE_CHANGE
#if ENABLED(BEEP_ON_FEEDRATE_CHANGE)
  #define FEEDRATE_CHANGE_BEEP_DURATION   10
  #define FEEDRATE_CHANGE_BEEP_FREQUENCY 440
#endif

/**
 * Probe Offset Wizard
 * Add a Probe Z Offset calibration option to the LCD menu.
 * Use this helper to get a perfect 'M851 Z' probe offset.
 * When launched this powerful wizard:
 *  - Measures the bed height at the configured position with the probe.
 *  - Moves the nozzle to the same position for a "paper" measurement.
 *  - The difference is used to set the probe Z offset.
 */
#if HAS_BED_PROBE && ANY(HAS_MARLINUI_MENU, HAS_TFT_LVGL_UI)
  #define PROBE_OFFSET_WIZARD
  #if ENABLED(PROBE_OFFSET_WIZARD)
    /**
     * Enable to init the Probe Z-Offset when starting the Wizard.
     * Use a height slightly above the estimated nozzle-to-probe Z offset.
     * For example, with an offset of -5, consider a starting height of -4.
     */
    #define PROBE_OFFSET_WIZARD_START_Z 0

    // Set a convenient position to do the calibration (probing point and nozzle/bed-distance)
    #define PROBE_OFFSET_WIZARD_XY_POS { X_CENTER, Y_CENTER }
  #endif
#endif

#if HAS_MARLINUI_MENU

  #if HAS_BED_PROBE

    // Show Deploy / Stow Probe options in the Motion menu.
    #define PROBE_DEPLOY_STOW_MENU

    // Add calibration in the Probe Offsets menu to compensate for X-axis twist.
    #define X_AXIS_TWIST_COMPENSATION
    #if ENABLED(X_AXIS_TWIST_COMPENSATION)
      /**
       * Enable to init the Probe Z-Offset when starting the Wizard.
       * Use a height slightly above the estimated nozzle-to-probe Z offset.
       * For example, with an offset of -5, consider a starting height of -4.
       */
      #define XATC_START_Z 0.0
      #define XATC_MAX_POINTS 3             // Number of points to probe in the wizard
      #define XATC_Y_POSITION Y_CENTER      // (mm) Y position to probe
      #define XATC_Z_OFFSETS { 0, 0, 0 }    // Z offsets for X axis sample points
    #endif

  #endif

  // Include a page of printer information in the LCD Main Menu
  #define LCD_INFO_MENU
  #if ENABLED(LCD_INFO_MENU)
    //#define LCD_PRINTER_INFO_IS_BOOTSCREEN // Show bootscreen(s) instead of Printer Info pages
  #endif

  /**
   * MarlinUI "Move Axis" menu distances. Comma-separated list.
   * Values are displayed as-defined, so always use plain numbers here.
   * Axis moves <= 1/2 the axis length and Extruder moves <= EXTRUDE_MAXLENGTH
   * will be shown in the move submenus.
   */

  // #define MANUAL_MOVE_DISTANCE_MM                    10, 1.0, 0.1  // (mm)
  #define MANUAL_MOVE_DISTANCE_MM         100, 50, 10, 1.0, 0.1  // (mm)
  //#define MANUAL_MOVE_DISTANCE_MM    500, 100, 50, 10, 1.0, 0.1  // (mm)

  // BACK menu items keep the highlight at the top
  //#define TURBO_BACK_MENU_ITEM

  // BACK menu items show "Back" instead of the previous menu name
  #define GENERIC_BACK_MENU_ITEM

  // Insert a menu for preheating at the top level to allow for quick access
  #define PREHEAT_SHORTCUT_MENU_ITEM

  // Add Configuration > Debug Menu > Endstop Test for endstop/probe/runout testing
  #define LCD_ENDSTOP_TEST

#endif // HAS_MARLINUI_MENU

#if HAS_DISPLAY
  /**
   * *** VENDORS PLEASE READ ***
   *
   * Marlin allows you to add a custom boot image for Graphical LCDs.
   * With this option Marlin will first show your custom screen followed
   * by the standard Marlin logo with version number and web URL.
   *
   * We encourage you to take advantage of this new feature and we also
   * respectfully request that you retain the unmodified Marlin boot screen.
   */
  #define SHOW_BOOTSCREEN                 // Show the Marlin bootscreen on startup. ** ENABLE FOR PRODUCTION **
  #if ENABLED(SHOW_BOOTSCREEN)
    #define BOOTSCREEN_TIMEOUT 3000       // (ms) Total Duration to display the boot screen(s)
    #if ANY(HAS_MARLINUI_U8GLIB, TFT_COLOR_UI)
      #define BOOT_MARLIN_LOGO_SMALL      // Show a smaller Marlin logo on the Boot Screen (saving lots of flash)
    #endif
    #if HAS_MARLINUI_U8GLIB
      //#define BOOT_MARLIN_LOGO_ANIMATED // Animated Marlin logo. Costs ~3260 (or ~940) bytes of flash.
    #endif
    #if ANY(HAS_MARLINUI_U8GLIB, TOUCH_UI_FTDI_EVE, HAS_MARLINUI_HD44780)
      #define SHOW_CUSTOM_BOOTSCREEN      // Show the bitmap in Marlin/_Bootscreen.h on startup.
    #endif
  #endif

  #if HAS_MARLINUI_U8GLIB
    #define CUSTOM_STATUS_SCREEN_IMAGE    // Show the bitmap in Marlin/_Statusscreen.h on the status screen.
  #endif

  #define SOUND_MENU_ITEM   // Add a mute option to the LCD menu
  #define SOUND_ON_DEFAULT    // Buzzer/speaker default enabled state

  #if HAS_WIRED_LCD
    // #define DOUBLE_LCD_FRAMERATE        // Not recommended for slow boards.
  #endif

  // The timeout to return to the status screen from sub-menus
  #define LCD_TIMEOUT_TO_STATUS 15000   // (ms)

  // Scroll a longer status message into view
  #define STATUS_MESSAGE_SCROLLING

  // Apply a timeout to low-priority status messages
  //#define STATUS_MESSAGE_TIMEOUT_SEC 30 // (seconds)

  // On the Info Screen, display XY with one decimal place when possible
  #define LCD_DECIMAL_SMALL_XY

  // Show the E position (filament used) during printing
  #define LCD_SHOW_E_TOTAL

#endif // HAS_DISPLAY

#if HAS_FEEDRATE_EDIT
  #define SPEED_EDIT_MIN    10  // (%) Feedrate percentage edit range minimum
  #define SPEED_EDIT_MAX   999  // (%) Feedrate percentage edit range maximum
#endif
#if HAS_FLOW_EDIT
  #define FLOW_EDIT_MIN     10  // (%) Flow percentage edit range minimum
  #define FLOW_EDIT_MAX    999  // (%) Flow percentage edit range maximum
#endif

// LCD Print Progress options. Multiple times may be displayed in turn.
#if HAS_DISPLAY && ANY(HAS_MEDIA, SET_PROGRESS_MANUALLY)
  #define SHOW_PROGRESS_PERCENT           // Show print progress percentage (doesn't affect progress bar)
  #define SHOW_ELAPSED_TIME               // Display elapsed printing time (prefix 'E')
  #define SHOW_REMAINING_TIME           // Display estimated time to completion (prefix 'R')
  #if ENABLED(SET_INTERACTION_TIME)
    #define SHOW_INTERACTION_TIME         // Display time until next user interaction ('C' = filament change)
  #endif
  #define PRINT_PROGRESS_SHOW_DECIMALS  // Show/report progress with decimal digits, not all UIs support this

  #if ANY(HAS_MARLINUI_HD44780, IS_TFTGLCD_PANEL)
    #define LCD_PROGRESS_BAR            // Show a progress bar on HD44780 LCDs for SD printing
    #if ENABLED(LCD_PROGRESS_BAR)
      #define PROGRESS_BAR_BAR_TIME 2000  // (ms) Amount of time to show the bar
      #define PROGRESS_BAR_MSG_TIME 3000  // (ms) Amount of time to show the status message
      #define PROGRESS_MSG_EXPIRE      0  // (ms) Amount of time to retain the status message (0=forever)
      //#define PROGRESS_MSG_ONCE         // Show the message for MSG_TIME then clear it
      #define LCD_PROGRESS_BAR_TEST     // Add a menu item to test the progress bar
    #endif
  #endif
#endif

#if HAS_MEDIA
  /**
   * SD Card SPI Speed
   * May be required to resolve "volume init" errors.
   *
   * Enable and set to SPI_HALF_SPEED, SPI_QUARTER_SPEED, or SPI_EIGHTH_SPEED
   *  otherwise full speed will be applied.
   *
   * :['SPI_HALF_SPEED', 'SPI_QUARTER_SPEED', 'SPI_EIGHTH_SPEED']
   */
  //#define SD_SPI_SPEED SPI_HALF_SPEED

  // The standard SD detect circuit reads LOW when media is inserted and HIGH when empty.
  // Enable this option and set to HIGH if your SD cards are incorrectly detected.
  //#define SD_DETECT_STATE HIGH

  //#define SD_IGNORE_AT_STARTUP            // Don't mount the SD card when starting up
  // #define SDCARD_READONLY                   // Read-only SD card (to save over 2K of flash)

  //#define GCODE_REPEAT_MARKERS            // Enable G-code M808 to set repeat markers and do looping

  #define SD_PROCEDURE_DEPTH 1              // Increase if you need more nested M32 calls

  #define SD_FINISHED_STEPPERRELEASE true   // Disable steppers when SD Print is finished
  #define SD_FINISHED_RELEASECOMMAND "M84"  // Use "M84XYE" to keep Z enabled so your bed stays in place

  // Reverse SD sort to show "more recent" files first, according to the card's FAT.
  // Since the FAT gets out of order with usage, SDCARD_SORT_ALPHA is recommended.
  // #define SDCARD_RATHERRECENTFIRST

  #define SD_MENU_CONFIRM_START             // Confirm the selected SD file before printing

  #define NO_SD_AUTOSTART                 // Remove auto#.g file support completely to save some Flash, SRAM
  //#define MENU_ADDAUTOSTART               // Add a menu option to run auto#.g files

  //#define ONE_CLICK_PRINT                 // Prompt to print the newest file on inserted media
  //#define BROWSE_MEDIA_ON_INSERT          // Open the file browser when media is inserted

  //#define MEDIA_MENU_AT_TOP               // Force the media menu to be listed on the top of the main menu

  #define EVENT_GCODE_SD_ABORT "G28XY"      // G-code to run on SD Abort Print (e.g., "G28XY" or "G27")

  /**
   * Continue after Power-Loss (Creality3D)
   *
   * Store the current state to the SD Card at the start of each layer
   * during SD printing. If the recovery file is found at boot time, present
   * an option on the LCD screen to continue the print from the last-known
   * point in the file.
   */
  #define POWER_LOSS_RECOVERY
  #if ENABLED(POWER_LOSS_RECOVERY)
    #define PLR_ENABLED_DEFAULT       true // Power-Loss Recovery enabled by default. (Set with 'M413 Sn' & M500)
    //#define PLR_BED_THRESHOLD BED_MAXTEMP // (°C) Skip user confirmation at or above this bed temperature (0 to disable)

    //#define POWER_LOSS_PIN             44 // Pin to detect power-loss. Set to -1 to disable default pin on boards without module, or comment to use board default.
    //#define POWER_LOSS_STATE         HIGH // State of pin indicating power-loss
    //#define POWER_LOSS_PULLUP             // Set pullup / pulldown as appropriate for your sensor
    //#define POWER_LOSS_PULLDOWN

    //#define POWER_LOSS_ZRAISE        2    // (mm) Z axis raise on resume (on power-loss with UPS)
    //#define POWER_LOSS_PURGE_LEN    20    // (mm) Length of filament to purge on resume

    // Without a POWER_LOSS_PIN the following option helps reduce wear on the SD card,
    // especially with "vase mode" printing. Set too high and vases cannot be continued.
    #define POWER_LOSS_MIN_Z_CHANGE    0.05 // (mm) Minimum Z change before saving power-loss data

    //#define BACKUP_POWER_SUPPLY           // Backup power / UPS to move the steppers on power-loss
    #if ENABLED(BACKUP_POWER_SUPPLY)
      //#define POWER_LOSS_RETRACT_LEN   10 // (mm) Length of filament to retract on fail
    #endif

    // Enable if Z homing is needed for proper recovery. 99.9% of the time this should be disabled!
    //#define POWER_LOSS_RECOVER_ZHOME
    #if ENABLED(POWER_LOSS_RECOVER_ZHOME)
      //#define POWER_LOSS_ZHOME_POS { 0, 0 } // Safe XY position to home Z while avoiding objects on the bed
    #endif
  #endif

  /**
   * Sort SD file listings in alphabetical order.
   *
   * With this option enabled, items on SD cards will be sorted
   * by name for easier navigation.
   *
   * By default...
   *
   *  - Use the slowest -but safest- method for sorting.
   *  - Folders are sorted to the top.
   *  - The sort key is statically allocated.
   *  - No added G-code (M34) support.
   *  - 40 item sorting limit. (Items after the first 40 are unsorted.)
   *
   * SD sorting uses static allocation (as set by SDSORT_LIMIT), allowing the
   * compiler to calculate the worst-case usage and throw an error if the SRAM
   * limit is exceeded.
   *
   *  - SDSORT_USES_RAM provides faster sorting via a static directory buffer.
   *  - SDSORT_USES_STACK does the same, but uses a local stack-based buffer.
   *  - SDSORT_CACHE_NAMES will retain the sorted file listing in RAM. (Expensive!)
   *  - SDSORT_DYNAMIC_RAM only uses RAM when the SD menu is visible. (Use with caution!)
   */
  // #define SDCARD_SORT_ALPHA

  // SD Card Sorting options
  #if ENABLED(SDCARD_SORT_ALPHA)
    #define SDSORT_REVERSE     false  // Default to sorting file names in reverse order.
    #define SDSORT_LIMIT       256     // Maximum number of sorted items (10-256). Costs 27 bytes each.
    #define SDSORT_FOLDERS     1     // -1=above  0=none  1=below
    #define SDSORT_GCODE       false  // Enable G-code M34 to set sorting behaviors: M34 S<-1|0|1> F<-1|0|1>
    #define SDSORT_USES_RAM    true  // Pre-allocate a static array for faster pre-sorting.
    #define SDSORT_USES_STACK  false  // Prefer the stack for pre-sorting to give back some SRAM. (Negated by next 2 options.)
    #define SDSORT_CACHE_NAMES true  // Keep sorted items in RAM longer for speedy performance. Most expensive option.
    #define SDSORT_DYNAMIC_RAM true  // Use dynamic allocation (within SD menus). Least expensive option. Set SDSORT_LIMIT before use!
    #define SDSORT_CACHE_VFATS 2      // Maximum number of 13-byte VFAT entries to use for sorting.
                                      // Note: Only affects SCROLL_LONG_FILENAMES with SDSORT_CACHE_NAMES but not SDSORT_DYNAMIC_RAM.
  #endif

  // Allow international symbols in long filenames. To display correctly, the
  // LCD's font must contain the characters. Check your selected LCD language.
  #define UTF_FILENAME_SUPPORT

  //#define LONG_FILENAME_HOST_SUPPORT    // Get the long filename of a file/folder with 'M33 <dosname>' and list long filenames with 'M20 L'
  //#define LONG_FILENAME_WRITE_SUPPORT   // Create / delete files with long filenames via M28, M30, and Binary Transfer Protocol
  //#define M20_TIMESTAMP_SUPPORT         // Include timestamps by adding the 'T' flag to M20 commands

  #define SCROLL_LONG_FILENAMES           // Scroll long filenames in the SD card menu

  //#define SD_ABORT_NO_COOLDOWN          // Leave the heaters on after Stop Print (not recommended!)

  /**
   * Abort SD printing when any endstop is triggered.
   * This feature is enabled with 'M540 S1' or from the LCD menu.
   * Endstops must be activated for this option to work.
   */
  //#define SD_ABORT_ON_ENDSTOP_HIT
  #if ENABLED(SD_ABORT_ON_ENDSTOP_HIT)
    //#define SD_ABORT_ON_ENDSTOP_HIT_GCODE "G28XY" // G-code to run on endstop hit (e.g., "G28XY" or "G27")
  #endif

  //#define SD_REPRINT_LAST_SELECTED_FILE // On print completion open the LCD Menu and select the same file

  #define AUTO_REPORT_SD_STATUS         // Auto-report media status with 'M27 S<seconds>'

  /**
   * Enable this option if you have more than ~3K of unused flash space.
   * Marlin will embed all settings in the firmware binary as compressed data.
   * Use 'M503 C' to write the settings out to the SD Card as 'mc.zip'.
   * See docs/ConfigEmbedding.md for details on how to use 'mc-apply.py'.
   */
  //#define CONFIGURATION_EMBEDDING

  // Add an optimized binary file transfer mode, initiated with 'M28 B1'
  //#define BINARY_FILE_TRANSFER

  #if ENABLED(BINARY_FILE_TRANSFER)
    // Include extra facilities (e.g., 'M20 F') supporting firmware upload via BINARY_FILE_TRANSFER
    //#define CUSTOM_FIRMWARE_UPLOAD
  #endif

  /**
   * Set this option to one of the following (or the board's defaults apply):
   *
   *           LCD - Use the SD drive in the external LCD controller.
   *       ONBOARD - Use the SD drive on the control board.
   *  CUSTOM_CABLE - Use a custom cable to access the SD (as defined in a pins file).
   *
   * :[ 'LCD', 'ONBOARD', 'CUSTOM_CABLE' ]
   */
  #define SDCARD_CONNECTION ONBOARD

  // Enable if SD detect is rendered useless (e.g., by using an SD extender)
  //#define NO_SD_DETECT

#endif // HAS_MEDIA

/**
 * By default an onboard SD card reader may be shared as a USB mass-
 * storage device. This option hides the SD card from the host PC.
 */
//#define NO_SD_HOST_DRIVE   // Disable SD Card access over USB (for security).

/**
 * Additional options for Graphical Displays
 *
 * Use the optimizations here to improve printing performance,
 * which can be adversely affected by graphical display drawing,
 * especially when doing several short moves, and when printing
 * on DELTA and SCARA machines.
 *
 * Some of these options may result in the display lagging behind
 * controller events, as there is a trade-off between reliable
 * printing performance versus fast display updates.
 */
#if HAS_MARLINUI_U8GLIB
  // Save many cycles by drawing a hollow frame or no frame on the Info Screen
  #define XYZ_NO_FRAME
  // #define XYZ_HOLLOW_FRAME

  // A bigger font is available for edit items. Costs 3120 bytes of flash.
  // Western only. Not available for Cyrillic, Kana, Turkish, Greek, or Chinese.
  //#define USE_BIG_EDIT_FONT

  // A smaller font may be used on the Info Screen. Costs 2434 bytes of flash.
  // Western only. Not available for Cyrillic, Kana, Turkish, Greek, or Chinese.
  //#define USE_SMALL_INFOFONT

  /**
   * ST7920-based LCDs can emulate a 16 x 4 character display using
   * the ST7920 character-generator for very fast screen updates.
   * Enable LIGHTWEIGHT_UI to use this special display mode.
   *
   * Since LIGHTWEIGHT_UI has limited space, the position and status
   * message occupy the same line. Set STATUS_EXPIRE_SECONDS to the
   * length of time to display the status message before clearing.
   *
   * Set STATUS_EXPIRE_SECONDS to zero to never clear the status.
   * This will prevent position updates from being displayed.
   */
  #if IS_U8GLIB_ST7920
    // Enable this option and reduce the value to optimize screen updates.
    // The normal delay is 10µs. Use the lowest value that still gives a reliable display.
    //#define DOGM_SPI_DELAY_US 5

    //#define LIGHTWEIGHT_UI
    #if ENABLED(LIGHTWEIGHT_UI)
      #define STATUS_EXPIRE_SECONDS 20
    #endif
  #endif

  /**
   * Status (Info) Screen customization
   * These options may affect code size and screen render time.
   * Custom status screens can forcibly override these settings.
   */
  //#define STATUS_COMBINE_HEATERS    // Use combined heater images instead of separate ones
  //#define STATUS_HOTEND_NUMBERLESS  // Use plain hotend icons instead of numbered ones (with 2+ hotends)
  #define STATUS_HOTEND_INVERTED      // Show solid nozzle bitmaps when heating (Requires STATUS_HOTEND_ANIM for numbered hotends)
  #define STATUS_HOTEND_ANIM          // Use a second bitmap to indicate hotend heating
  #define STATUS_BED_ANIM             // Use a second bitmap to indicate bed heating
  #define STATUS_ALT_BED_BITMAP     // Use the alternative bed bitmap
  #define STATUS_ALT_FAN_BITMAP     // Use the alternative fan bitmap
  #define STATUS_FAN_FRAMES 4       // :[0,1,2,3,4] Number of fan animation frames

  // Only one STATUS_HEAT_* option can be enabled
  // #define STATUS_HEAT_PERCENT       // Show heating in a progress bar
  #define STATUS_HEAT_POWER         // Show heater output power as a vertical bar

#endif // HAS_MARLINUI_U8GLIB

#if HAS_MARLINUI_U8GLIB || IS_DWIN_MARLINUI
  #define MENU_HOLLOW_FRAME           // Enable to save many cycles by drawing a hollow frame on Menu Screens
  //#define OVERLAY_GFX_REVERSE       // Swap the CW/CCW indicators in the graphics overlay

  // Frivolous Game Options
  //#define MARLIN_BRICKOUT
  //#define MARLIN_INVADERS
  //#define MARLIN_SNAKE
  //#define GAMES_EASTER_EGG          // Add extra blank lines above the "Games" sub-menu
#endif

// @section safety

/**
 * The watchdog hardware timer will do a reset and disable all outputs
 * if the firmware gets too overloaded to read the temperature sensors.
 *
 * If you find that watchdog reboot causes your AVR board to hang forever,
 * enable WATCHDOG_RESET_MANUAL to use a custom timer instead of WDTO.
 * NOTE: This method is less reliable as it can only catch hangups while
 * interrupts are enabled.
 */
#define USE_WATCHDOG
#if ENABLED(USE_WATCHDOG)
  //#define WATCHDOG_RESET_MANUAL
#endif

// @section lcd

/**
 * Babystepping enables movement of the axes by tiny increments without changing
 * the current position values. This feature is used primarily to adjust the Z
 * axis in the first layer of a print in real-time.
 *
 * Warning: Does not respect endstops!
 */
#define BABYSTEPPING
#if ENABLED(BABYSTEPPING)
  #define EP_BABYSTEPPING                 // M293/M294 babystepping with EMERGENCY_PARSER support
  #define BABYSTEP_WITHOUT_HOMING
  #define BABYSTEP_ALWAYS_AVAILABLE       // Allow babystepping at all times (not just during movement)
  //#define BABYSTEP_XY                     // Also enable X/Y Babystepping. Not supported on DELTA!
  //#define BABYSTEP_INVERT_Z               // Enable if Z babysteps should go the other way
  #define BABYSTEP_MILLIMETER_UNITS       // Specify BABYSTEP_MULTIPLICATOR_(XY|Z) in mm instead of micro-steps
  #define BABYSTEP_MULTIPLICATOR_Z  .01       // (steps or mm) Steps or millimeter distance for each Z babystep
  #define BABYSTEP_MULTIPLICATOR_XY .05       // (steps or mm) Steps or millimeter distance for each XY babystep

  // #define DOUBLECLICK_FOR_Z_BABYSTEPPING    // Double-click on the Status Screen for Z Babystepping.
  #if ENABLED(DOUBLECLICK_FOR_Z_BABYSTEPPING)
    #define DOUBLECLICK_MAX_INTERVAL 1250   // Maximum interval between clicks, in milliseconds.
                                            // Note: Extra time may be added to mitigate controller latency.
    //#define MOVE_Z_WHEN_IDLE              // Jump to the move Z menu on double-click when printer is idle.
    #if ENABLED(MOVE_Z_WHEN_IDLE)
      #define MOVE_Z_IDLE_MULTIPLICATOR 1   // Multiply 1mm by this factor for the move step size.
    #endif
  #endif

  #define BABYSTEP_DISPLAY_TOTAL          // Display total babysteps since last G28

  #define BABYSTEP_ZPROBE_OFFSET          // Combine M851 Z and Babystepping
  #define BABYSTEP_GLOBAL_Z               // Combine M424 Z and Babystepping

  #if ANY(BABYSTEP_ZPROBE_OFFSET, BABYSTEP_GLOBAL_Z)
    #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
      //#define BABYSTEP_HOTEND_Z_OFFSET    // For multiple hotends, babystep relative Z offsets
    #endif
    #define BABYSTEP_GFX_OVERLAY          // Enable graphical overlay on Z-offset editor
  #endif
#endif

// @section extruder

/**
 * Nonlinear Extrusion Control
 *
 * Control extrusion rate based on instantaneous extruder velocity. Can be used to correct for
 * underextrusion at high extruder speeds that are otherwise well-behaved (i.e., not skipping).
 * For better results also enable ADAPTIVE_STEP_SMOOTHING.
 */
#define NONLINEAR_EXTRUSION

// @section leveling

/**
 * Use Safe Bed Leveling coordinates to move axes to a useful position before bed probing.
 * For example, after homing a rotational axis the Z probe might not be perpendicular to the bed.
 * Choose values the orient the bed horizontally and the Z-probe vertically.
 */
#define SAFE_BED_LEVELING_START_X 10.0
#define SAFE_BED_LEVELING_START_Y 10.0
#define SAFE_BED_LEVELING_START_Z 4.0

/**
 * Points to probe for all 3-point Leveling procedures.
 * Override if the automatically selected points are inadequate.
 */
#if NEEDS_THREE_PROBE_POINTS
  //#define PROBE_PT_1 {  15, 180 }   // (mm) { x, y }
  //#define PROBE_PT_2 {  15,  20 }
  //#define PROBE_PT_3 { 170,  20 }
#endif

/**
 * Probing Margins
 *
 * Override PROBING_MARGIN for each side of the build plate
 * Useful to get probe points to exact positions on targets or
 * to allow leveling to avoid plate clamps on only specific
 * sides of the bed. With NOZZLE_AS_PROBE negative values are
 * allowed, to permit probing outside the bed.
 *
 * If you are replacing the prior *_PROBE_BED_POSITION options,
 * LEFT and FRONT values in most cases will map directly over
 * RIGHT and REAR would be the inverse such as
 * (X/Y_BED_SIZE - RIGHT/BACK_PROBE_BED_POSITION)
 *
 * This will allow all positions to match at compilation, however
 * should the probe position be modified with M851XY then the
 * probe points will follow. This prevents any change from causing
 * the probe to be unable to reach any points.
 */
#if PROBE_SELECTED && !IS_KINEMATIC
  #define PROBING_MARGIN_LEFT 5
  #define PROBING_MARGIN_RIGHT 5
  #define PROBING_MARGIN_FRONT 10
  #define PROBING_MARGIN_BACK  10
#endif

#if ANY(MESH_BED_LEVELING, AUTO_BED_LEVELING_UBL)
  // Override the mesh area if the automatic (max) area is too large
  #define MESH_MIN_X MESH_INSET
  #define MESH_MIN_Y MESH_INSET
  #define MESH_MAX_X X_BED_SIZE - (MESH_INSET)
  #define MESH_MAX_Y Y_BED_SIZE - (MESH_INSET)
#endif

#if ALL(AUTO_BED_LEVELING_UBL, EEPROM_SETTINGS)
  //#define OPTIMIZED_MESH_STORAGE  // Store mesh with less precision to save EEPROM space
#endif

/**
 * Repeatedly attempt G29 leveling until it succeeds.
 * Stop after G29_MAX_RETRIES attempts.
 */
// #define G29_RETRY_AND_RECOVER
#if ENABLED(G29_RETRY_AND_RECOVER)
  #define G29_MAX_RETRIES 3
  #define G29_HALT_ON_FAILURE
  /**
   * Specify the GCODE commands that will be executed when leveling succeeds,
   * between attempts, and after the maximum number of retries have been tried.
   */
  #define G29_SUCCESS_COMMANDS "M117 Bed leveling done."
  #define G29_RECOVER_COMMANDS "M117 Probe failed. Rewiping.\nG28\nG12 P0 S12 T0"
  #define G29_FAILURE_COMMANDS "M117 Bed leveling failed.\nG0 Z10\nM300 P25 S880\nM300 P50 S0\nM300 P25 S880\nM300 P50 S0\nM300 P25 S880\nM300 P50 S0\nG4 S1"

#endif

// @section extras

//
// G60/G61 Position Save and Return
//
//#define SAVED_POSITIONS 1         // Each saved position slot costs 12 bytes

//
// G2/G3 Arc Support
//
#define ARC_SUPPORT                   // Requires ~3226 bytes
#if ENABLED(ARC_SUPPORT)
  #define MIN_ARC_SEGMENT_MM      0.1 // (mm) Minimum length of each arc segment
  #define MAX_ARC_SEGMENT_MM      1.0 // (mm) Maximum length of each arc segment
  #define MIN_CIRCLE_SEGMENTS    72   // Minimum number of segments in a complete circle
  //#define ARC_SEGMENTS_PER_SEC 50   // Use the feedrate to choose the segment length
  #define N_ARC_CORRECTION       25   // Number of interpolated segments between corrections
  //#define ARC_P_CIRCLES             // Enable the 'P' parameter to specify complete circles
  //#define SF_ARC_FIX                // Enable only if using SkeinForge with "Arc Point" fillet procedure
#endif

// G5 Bézier Curve Support with XYZE destination and IJPQ offsets
#define BEZIER_CURVE_SUPPORT        // Requires ~2666 bytes

/**
 * Direct Stepping
 *
 * Comparable to the method used by Klipper, G6 direct stepping significantly
 * reduces motion calculations, increases top printing speeds, and results in
 * less step aliasing by calculating all motions in advance.
 * Preparing your G-code: https://github.com/colinrgodsey/step-daemon
 */
//#define DIRECT_STEPPING

/**
 * G38 Probe Target
 *
 * This option adds G38.2 and G38.3 (probe towards target)
 * and optionally G38.4 and G38.5 (probe away from target).
 * Set MULTIPLE_PROBING for G38 to probe more than once.
 */
//#define G38_PROBE_TARGET
#if ENABLED(G38_PROBE_TARGET)
  //#define G38_PROBE_AWAY        // Include G38.4 and G38.5 to probe away from target
  #define G38_MINIMUM_MOVE 0.0275 // (mm) Minimum distance that will produce a move.
#endif

// @section motion

// Moves (or segments) with fewer steps than this will be joined with the next move
#define MIN_STEPS_PER_SEGMENT 1

/**
 * Minimum delay before and after setting the stepper DIR (in ns)
 *     0 : No delay (Expect at least 10µS since one Stepper ISR must transpire)
 *    20 : Minimum for TMC2xxx drivers
 *   200 : Minimum for A4988 drivers
 *   400 : Minimum for A5984 drivers
 *   500 : Minimum for LV8729 drivers (guess, no info in datasheet)
 *   650 : Minimum for DRV8825 drivers
 *  1500 : Minimum for TB6600 drivers (guess, no info in datasheet)
 * 15000 : Minimum for TB6560 drivers (guess, no info in datasheet)
 *
 * Override the default value based on the driver type set in Configuration.h.
 */
//#define MINIMUM_STEPPER_POST_DIR_DELAY 650
//#define MINIMUM_STEPPER_PRE_DIR_DELAY 650

/**
 * Minimum stepper driver pulse width (in ns)
 * If undefined, these defaults (from Conditionals-4-adv.h) apply:
 *     100 : Minimum for TMC2xxx stepper drivers
 *     500 : Minimum for LV8729
 *    1000 : Minimum for A4988 and A5984 stepper drivers
 *    2000 : Minimum for DRV8825 stepper drivers
 *    3000 : Minimum for TB6600 stepper drivers
 *   30000 : Minimum for TB6560 stepper drivers
 *
 * Override the default value based on the driver type set in Configuration.h.
 */
//#define MINIMUM_STEPPER_PULSE_NS 2000

/**
 * Maximum stepping rate (in Hz) the stepper driver allows
 * If undefined, these defaults (from Conditionals-4-adv.h) apply:
 *  5000000 : Maximum for TMC2xxx stepper drivers
 *  1000000 : Maximum for LV8729 stepper driver
 *   500000 : Maximum for A4988 stepper driver
 *   250000 : Maximum for DRV8825 stepper driver
 *   150000 : Maximum for TB6600 stepper driver
 *    15000 : Maximum for TB6560 stepper driver
 *
 * Override the default value based on the driver type set in Configuration.h.
 */
//#define MAXIMUM_STEPPER_RATE 250000

// @section temperature

// Control heater 0 and heater 1 in parallel.
//#define HEATERS_PARALLEL

//===========================================================================
//================================= Buffers =================================
//===========================================================================

// @section gcode

// The number of linear moves that can be in the planner at once.
#if ALL(HAS_MEDIA, DIRECT_STEPPING)
  #define BLOCK_BUFFER_SIZE  8
#elif HAS_MEDIA
  #define BLOCK_BUFFER_SIZE 16
#else
  #define BLOCK_BUFFER_SIZE 16
#endif

// @section serial

// The ASCII buffer for serial input
#define MAX_CMD_SIZE 96
#define BUFSIZE 4

// Transmission to Host Buffer Size
// To save 386 bytes of flash (and TX_BUFFER_SIZE+3 bytes of RAM) set to 0.
// To buffer a simple "ok" you need 4 bytes.
// For ADVANCED_OK (M105) you need 32 bytes.
// For debug-echo: 128 bytes for the optimal speed.
// Other output doesn't need to be that speedy.
// :[0, 2, 4, 8, 16, 32, 64, 128, 256]
#define TX_BUFFER_SIZE 0

// Host Receive Buffer Size
// Without XON/XOFF flow control (see SERIAL_XON_XOFF below) 32 bytes should be enough.
// To use flow control, set this buffer size to at least 1024 bytes.
// :[0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048]
//#define RX_BUFFER_SIZE 1024

#if RX_BUFFER_SIZE >= 1024
  // Enable to have the controller send XON/XOFF control characters to
  // the host to signal the RX buffer is becoming full.
  //#define SERIAL_XON_XOFF
#endif

#if HAS_MEDIA
  // Enable this option to collect and display the maximum
  // RX queue usage after transferring a file to SD.
  //#define SERIAL_STATS_MAX_RX_QUEUED

  // Enable this option to collect and display the number
  // of dropped bytes after a file transfer to SD.
  //#define SERIAL_STATS_DROPPED_RX
#endif

// Monitor RX buffer usage
// Dump an error to the serial port if the serial receive buffer overflows.
// If you see these errors, increase the RX_BUFFER_SIZE value.
// Not supported on all platforms.
//#define RX_BUFFER_MONITOR

/**
 * Emergency Command Parser
 *
 * Add a low-level parser to intercept certain commands as they
 * enter the serial receive buffer, so they cannot be blocked.
 * Currently handles M108, M112, M410, M876
 * NOTE: Not yet implemented for all platforms.
 */
#define EMERGENCY_PARSER

/**
 * Realtime Reporting (requires EMERGENCY_PARSER)
 *
 * - Report position and state of the machine (like Grbl).
 * - Auto-report position during long moves.
 * - Useful for CNC/LASER.
 *
 * Adds support for commands:
 *  S000 : Report State and Position while moving.
 *  P000 : Instant Pause / Hold while moving.
 *  R000 : Resume from Pause / Hold.
 *
 * - During Hold all Emergency Parser commands are available, as usual.
 * - Enable NANODLP_Z_SYNC and NANODLP_ALL_AXIS for move command end-state reports.
 */
// #define REALTIME_REPORTING_COMMANDS
#if ENABLED(REALTIME_REPORTING_COMMANDS)
  //#define FULL_REPORT_TO_HOST_FEATURE   // Auto-report the machine status like Grbl CNC
#endif

/**
 * Bad Serial-connections can miss a received command by sending an 'ok'
 * Therefore some clients abort after 30 seconds in a timeout.
 * Some other clients start sending commands while receiving a 'wait'.
 * This "wait" is only sent when the buffer is empty. 1 second is a good value here.
 */
//#define NO_TIMEOUTS 1000 // (ms)

// Some clients will have this feature soon. This could make the NO_TIMEOUTS unnecessary.
#define ADVANCED_OK

// Printrun may have trouble receiving long strings all at once.
// This option inserts short delays between lines of serial output.
#define SERIAL_OVERRUN_PROTECTION

// For serial echo, the number of digits after the decimal point
#define SERIAL_FLOAT_PRECISION 2

/**
 * This feature is EXPERIMENTAL so use with caution and test thoroughly.
 * Enable this option to receive data on the serial ports via the onboard DMA
 * controller for more stable and reliable high-speed serial communication.
 * Support is currently limited to some STM32 MCUs and all HC32 MCUs.
 * Note: This has no effect on emulated USB serial ports.
 */
#define SERIAL_DMA

/**
 * Set the number of proportional font spaces required to fill up a typical character space.
 * This can help to better align the output of commands like 'G29 O' Mesh Output.
 *
 * For clients that use a fixed-width font (like OctoPrint), leave this set to 1.0.
 * Otherwise, adjust according to your client and font.
 */
#define PROPORTIONAL_FONT_RATIO 1.0

// @section extras

/**
 * Extra Fan Speed
 * Adds a secondary fan speed for each print-cooling fan.
 *   'M106 P<fan> T3-255' : Set a secondary speed for <fan>
 *   'M106 P<fan> T2'     : Use the set secondary speed
 *   'M106 P<fan> T1'     : Restore the previous fan speed
 */
//#define EXTRA_FAN_SPEED

// @section gcode

/**
 * Firmware-based and LCD-controlled retract
 *
 * Add G10 / G11 commands for automatic firmware-based retract / recover.
 * Use M207 and M208 to define parameters for retract / recover.
 *
 * Use M209 to enable or disable auto-retract.
 * With auto-retract enabled, all G1 E moves within the set range
 * will be converted to firmware-based retract/recover moves.
 *
 * Be sure to turn off auto-retract during filament change.
 *
 * Note that M207 / M208 / M209 settings are saved to EEPROM.
 */
#define FWRETRACT
#if ENABLED(FWRETRACT)
  #define FWRETRACT_AUTORETRACT             // Override slicer retractions
  #if ENABLED(FWRETRACT_AUTORETRACT)
    #define MIN_AUTORETRACT             0.1 // (mm) Don't convert E moves under this length
    #define MAX_AUTORETRACT            10.0 // (mm) Don't convert E moves over this length
  #endif
  #define RETRACT_LENGTH                1   // (mm) Default retract length (positive value)
  #define RETRACT_LENGTH_SWAP          13   // (mm) Default swap retract length (positive value)
  #define RETRACT_FEEDRATE             40   // (mm/s) Default feedrate for retracting
  #define RETRACT_ZRAISE                0.5   // (mm) Default retract Z-raise
  #define RETRACT_RECOVER_LENGTH        0   // (mm) Default additional recover length (added to retract length on recover)
  #define RETRACT_RECOVER_LENGTH_SWAP   0   // (mm) Default additional swap recover length (added to retract length on recover from toolchange)
  #define RETRACT_RECOVER_FEEDRATE     15   // (mm/s) Default feedrate for recovering from retraction
  #define RETRACT_RECOVER_FEEDRATE_SWAP 8   // (mm/s) Default feedrate for recovering from swap retraction
#endif

// @section advanced pause

/**
 * Advanced Pause for Filament Change
 *  - Adds the G-code M600 Filament Change to initiate a filament change.
 *  - This feature is required for the default FILAMENT_RUNOUT_SCRIPT.
 *
 * Requirements:
 *  - For Filament Change parking enable and configure NOZZLE_PARK_FEATURE.
 *  - For user interaction enable an LCD display, HOST_PROMPT_SUPPORT, or EMERGENCY_PARSER.
 *
 * Enable PARK_HEAD_ON_PAUSE to add the G-code M125 Pause and Park.
 */
#define ADVANCED_PAUSE_FEATURE
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #define PAUSE_PARK_RETRACT_FEEDRATE         60  // (mm/s) Initial retract feedrate.
  #define PAUSE_PARK_RETRACT_LENGTH            2  // (mm) Initial retract.
                                                  // This short retract is done immediately, before parking the nozzle.
  #define FILAMENT_CHANGE_UNLOAD_FEEDRATE     10  // (mm/s) Unload filament feedrate. This can be pretty fast.
  #define FILAMENT_CHANGE_UNLOAD_ACCEL        25  // (mm/s^2) Lower acceleration may allow a faster feedrate.
  #define FILAMENT_CHANGE_UNLOAD_LENGTH      450  // (mm) The length of filament for a complete unload.
                                                  //   For Bowden, the full length of the tube and nozzle.
                                                  //   For direct drive, the full length of the nozzle.
                                                  //   Set to 0 for manual unloading.
  #define FILAMENT_CHANGE_SLOW_LOAD_FEEDRATE   6  // (mm/s) Slow move when starting load.
  #define FILAMENT_CHANGE_SLOW_LOAD_LENGTH     40  // (mm) Slow length, to allow time to insert material.
                                                  // 0 to disable start loading and skip to fast load only
  #define FILAMENT_CHANGE_FAST_LOAD_FEEDRATE   6  // (mm/s) Load filament feedrate. This can be pretty fast.
  #define FILAMENT_CHANGE_FAST_LOAD_ACCEL     25  // (mm/s^2) Lower acceleration may allow a faster feedrate.
  #define FILAMENT_CHANGE_FAST_LOAD_LENGTH     400  // (mm) Load length of filament, from extruder gear to nozzle.
                                                  //   For Bowden, the full length of the tube and nozzle.
                                                  //   For direct drive, the full length of the nozzle.
  //#define ADVANCED_PAUSE_CONTINUOUS_PURGE       // Purge continuously up to the purge length until interrupted.
  #define ADVANCED_PAUSE_PURGE_FEEDRATE        3  // (mm/s) Extrude feedrate (after loading). Should be slower than load feedrate.
  #define ADVANCED_PAUSE_PURGE_LENGTH         50  // (mm) Length to extrude after loading.
                                                  //   Set to 0 for manual extrusion.
                                                  //   Filament can be extruded repeatedly from the Filament Change menu
                                                  //   until extrusion is consistent, and to purge old filament.
  #define ADVANCED_PAUSE_RESUME_PRIME          10  // (mm) Extra distance to prime nozzle after returning from park.
  //#define ADVANCED_PAUSE_FANS_PAUSE             // Turn off print-cooling fans while the machine is paused.

                                                  // Filament Unload does a Retract, Delay, and Purge first:
  #define FILAMENT_UNLOAD_PURGE_RETRACT       13  // (mm) Unload initial retract length.
  #define FILAMENT_UNLOAD_PURGE_DELAY       5000  // (ms) Delay for the filament to cool after retract.
  #define FILAMENT_UNLOAD_PURGE_LENGTH         8  // (mm) An unretract is done, then this length is purged.
  #define FILAMENT_UNLOAD_PURGE_FEEDRATE      25  // (mm/s) feedrate to purge before unload

  #define PAUSE_PARK_NOZZLE_TIMEOUT           45  // (seconds) Time limit before the nozzle is turned off for safety.
  #define FILAMENT_CHANGE_ALERT_BEEPS         10  // Number of alert beeps to play when a response is needed.
  #define PAUSE_PARK_NO_STEPPER_TIMEOUT           // Enable for XYZ steppers to stay powered on during filament change.
  //#define FILAMENT_CHANGE_RESUME_ON_INSERT      // Automatically continue / load filament when runout sensor is triggered again.
  #define PAUSE_REHEAT_FAST_RESUME              // Reduce number of waits by not prompting again post-timeout before continuing.

  #define PARK_HEAD_ON_PAUSE                    // Park the nozzle during pause and filament change.
  //#define HOME_BEFORE_FILAMENT_CHANGE           // If needed, home before parking for filament change

  #define FILAMENT_LOAD_UNLOAD_GCODES           // Add M701/M702 Load/Unload G-codes, plus Load/Unload in the LCD Prepare menu.
  //#define FILAMENT_UNLOAD_ALL_EXTRUDERS         // Allow M702 to unload all extruders above a minimum target temp (as set by M302)
  // #define CONFIGURE_FILAMENT_CHANGE               // Add M603 G-code and menu items. Requires ~1.3K bytes of flash.
#endif

// @section power

/**
 * Power Monitor
 * Monitor voltage (V) and/or current (A), and -when possible- power (W)
 *
 * Read and configure with M430
 *
 * The current sensor feeds DC voltage (relative to the measured current) to an analog pin
 * The voltage sensor feeds DC voltage (relative to the measured voltage) to an analog pin
 */
//#define POWER_MONITOR_CURRENT   // Monitor the system current
//#define POWER_MONITOR_VOLTAGE   // Monitor the system voltage

#if ENABLED(POWER_MONITOR_CURRENT)
  #define POWER_MONITOR_VOLTS_PER_AMP    0.05000  // Input voltage to the MCU analog pin per amp  - DO NOT apply more than ADC_VREF!
  #define POWER_MONITOR_CURRENT_OFFSET   0        // Offset (in amps) applied to the calculated current
  #define POWER_MONITOR_FIXED_VOLTAGE   13.6      // Voltage for a current sensor with no voltage sensor (for power display)
#endif

#if ENABLED(POWER_MONITOR_VOLTAGE)
  #define POWER_MONITOR_VOLTS_PER_VOLT  0.077933  // Input voltage to the MCU analog pin per volt - DO NOT apply more than ADC_VREF!
  #define POWER_MONITOR_VOLTAGE_OFFSET  0         // Offset (in volts) applied to the calculated voltage
#endif

// @section safety


// @section security

/**
 * Expected Printer Check
 * Add the M16 G-code to compare a string to the MACHINE_NAME.
 * M16 with a non-matching string causes the printer to halt.
 */
//#define EXPECTED_PRINTER_CHECK

// @section volumetrics

/**
 * Disable all Volumetric extrusion options
 */
//#define NO_VOLUMETRICS

#if DISABLED(NO_VOLUMETRICS)
  /**
   * Volumetric extrusion default state
   * Activate to make volumetric extrusion the default method,
   * with DEFAULT_NOMINAL_FILAMENT_DIA as the default diameter.
   *
   * M200 D0 to disable, M200 Dn to set a new diameter (and enable volumetric).
   * M200 S0/S1 to disable/enable volumetric extrusion.
   */
  #define VOLUMETRIC_DEFAULT_ON

  #define VOLUMETRIC_EXTRUDER_LIMIT
  #if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
    /**
     * Default volumetric extrusion limit in cubic mm per second (mm^3/sec).
     * This factory setting applies to all extruders.
     * Use 'M200 [T<extruder>] L<limit>' to override and 'M502' to reset.
     * A non-zero value activates Volume-based Extrusion Limiting.
     */
    #define DEFAULT_VOLUMETRIC_EXTRUDER_LIMIT  0     // (mm^3/sec)
    #define VOLUMETRIC_EXTRUDER_LIMIT_MAX     20        // (mm^3/sec)
  #endif
#endif

// @section reporting

/**
 * Extra options for the M114 "Current Position" report
 */
#define M114_DETAIL         // Use 'M114` for details to check planner calculations
//#define M114_REALTIME       // Real current position based on forward kinematics
//#define M114_LEGACY         // M114 used to synchronize on every call. Enable if needed.

/**
 * Auto-report fan speed with M123 S<seconds>
 * Requires fans with tachometer pins
 */
//#define AUTO_REPORT_FANS

//#define REPORT_FAN_CHANGE   // Report the new fan speed when changed by M106 (and others)

/**
 * Auto-report temperatures with M155 S<seconds>
 */
#define AUTO_REPORT_TEMPERATURES
#if ENABLED(AUTO_REPORT_TEMPERATURES) && TEMP_SENSOR_REDUNDANT
  //#define AUTO_REPORT_REDUNDANT // Include the "R" sensor in the auto-report
#endif

/**
 * Auto-report position with M154 S<seconds>
 */
#define AUTO_REPORT_POSITION
#if ENABLED(AUTO_REPORT_POSITION)
  #define AUTO_REPORT_REAL_POSITION // Auto-report the real position
#endif

/**
 * M115 - Report capabilites. Disable to save ~1150 bytes of flash.
 *        Some hosts (and serial TFT displays) rely on this feature.
 */
// #define CAPABILITIES_REPORT
#if ENABLED(CAPABILITIES_REPORT)
  // Include capabilities in M115 output
  #define EXTENDED_CAPABILITIES_REPORT
  #if ENABLED(EXTENDED_CAPABILITIES_REPORT)
    #define M115_GEOMETRY_REPORT
  #endif
#endif

// @section gcode

/**
 * Spend 28 bytes of SRAM to optimize the G-code parser
 */
#define FASTER_GCODE_PARSER

#if ENABLED(FASTER_GCODE_PARSER)
  #define GCODE_QUOTED_STRINGS  // Support for quoted string parameters
#endif

/**
 * Variables
 *
 * Define a variable from 100-115 with G-code like '#101=19.6'.
 * A variable can then be used in a G-code expression like 'G0 X[#101+3]'.
 * See https://gcodetutor.com/cnc-macro-programming/cnc-variables.html
 */
//#define GCODE_VARIABLES

/**
 * Support for MeatPack G-code compression (https://github.com/scottmudge/OctoPrint-MeatPack)
 */
// #define MEATPACK_ON_SERIAL_PORT_1
//#define MEATPACK_ON_SERIAL_PORT_2

#define GCODE_CASE_INSENSITIVE  // Accept G-code sent to the firmware in lowercase

#define REPETIER_GCODE_M360     // Add commands originally from Repetier FW

/**
 * Enable M111 debug flags 1=ECHO, 2=INFO, 4=ERRORS (unimplemented).
 * Disable to save some flash. Some hosts (Repetier Host) may rely on this feature.
 */
// #define DEBUG_FLAGS_GCODE

/**
 * Enable this option for a leaner build of Marlin that removes
 * workspace offsets to slightly optimize performance.
 * G92 will revert to its behavior from Marlin 1.0.
 */
//#define NO_WORKSPACE_OFFSETS

/**
 * Disable M206 and M428 if you don't need home offsets.
 */
#define NO_HOME_OFFSETS

// Enable and set a (default) feedrate for all G0 moves
//#define G0_FEEDRATE 3000 // (mm/min)
#ifdef G0_FEEDRATE
  //#define VARIABLE_G0_FEEDRATE // The G0 feedrate is set by F in G0 motion mode
#endif

/**
 * Startup commands
 *
 * Execute certain G-code commands immediately after power-on.
 */
//#define STARTUP_COMMANDS "M17 Z"

/**
 * G-code Macros
 *
 * Add G-codes M810-M819 to define and run G-code macros.
 * Macros are not saved to EEPROM.
 */
//#define GCODE_MACROS
#if ENABLED(GCODE_MACROS)
  #define GCODE_MACROS_SLOTS       5  // Up to 10 may be used
  #define GCODE_MACROS_SLOT_SIZE  50  // Maximum length of a single macro
#endif

/**
 * User-defined menu items to run custom G-code.
 * Up to 25 may be defined, but the actual number is LCD-dependent.
 */

// @section custom main menu

// Custom Menu: Main Menu
//#define CUSTOM_MENU_MAIN
#if ENABLED(CUSTOM_MENU_MAIN)
  //#define CUSTOM_MENU_MAIN_TITLE "Custom Commands"
  #define CUSTOM_MENU_MAIN_SCRIPT_DONE "M117 User Script Done"
  #define CUSTOM_MENU_MAIN_SCRIPT_AUDIBLE_FEEDBACK
  //#define CUSTOM_MENU_MAIN_SCRIPT_RETURN   // Return to status screen after a script
  #define CUSTOM_MENU_MAIN_ONLY_IDLE         // Only show custom menu when the machine is idle

  #define MAIN_MENU_ITEM_1_DESC "Home & UBL Info"
  #define MAIN_MENU_ITEM_1_GCODE "G28\nG29 W"
  //#define MAIN_MENU_ITEM_1_CONFIRM          // Show a confirmation dialog before this action

  #define MAIN_MENU_ITEM_2_DESC "Preheat for " PREHEAT_1_LABEL
  #define MAIN_MENU_ITEM_2_GCODE "M140 S" STRINGIFY(PREHEAT_1_TEMP_BED) "\nM104 S" STRINGIFY(PREHEAT_1_TEMP_HOTEND)
  //#define MAIN_MENU_ITEM_2_CONFIRM

  //#define MAIN_MENU_ITEM_3_DESC "Preheat for " PREHEAT_2_LABEL
  //#define MAIN_MENU_ITEM_3_GCODE "M140 S" STRINGIFY(PREHEAT_2_TEMP_BED) "\nM104 S" STRINGIFY(PREHEAT_2_TEMP_HOTEND)
  //#define MAIN_MENU_ITEM_3_CONFIRM

  //#define MAIN_MENU_ITEM_4_DESC "Heat Bed/Home/Level"
  //#define MAIN_MENU_ITEM_4_GCODE "M140 S" STRINGIFY(PREHEAT_2_TEMP_BED) "\nG28\nG29"
  //#define MAIN_MENU_ITEM_4_CONFIRM

  //#define MAIN_MENU_ITEM_5_DESC "Home & Info"
  //#define MAIN_MENU_ITEM_5_GCODE "G28\nM503"
  //#define MAIN_MENU_ITEM_5_CONFIRM
#endif

// @section custom config menu

// Custom Menu: Configuration Menu
//#define CUSTOM_MENU_CONFIG
#if ENABLED(CUSTOM_MENU_CONFIG)
  //#define CUSTOM_MENU_CONFIG_TITLE "Custom Commands"
  #define CUSTOM_MENU_CONFIG_SCRIPT_DONE "M117 Wireless Script Done"
  #define CUSTOM_MENU_CONFIG_SCRIPT_AUDIBLE_FEEDBACK
  //#define CUSTOM_MENU_CONFIG_SCRIPT_RETURN  // Return to status screen after a script
  #define CUSTOM_MENU_CONFIG_ONLY_IDLE        // Only show custom menu when the machine is idle

  #define CONFIG_MENU_ITEM_1_DESC "Wifi ON"
  #define CONFIG_MENU_ITEM_1_GCODE "M118 [ESP110] WIFI-STA pwd=12345678"
  //#define CONFIG_MENU_ITEM_1_CONFIRM        // Show a confirmation dialog before this action

  #define CONFIG_MENU_ITEM_2_DESC "Bluetooth ON"
  #define CONFIG_MENU_ITEM_2_GCODE "M118 [ESP110] BT pwd=12345678"
  //#define CONFIG_MENU_ITEM_2_CONFIRM

  //#define CONFIG_MENU_ITEM_3_DESC "Radio OFF"
  //#define CONFIG_MENU_ITEM_3_GCODE "M118 [ESP110] OFF pwd=12345678"
  //#define CONFIG_MENU_ITEM_3_CONFIRM

  //#define CONFIG_MENU_ITEM_4_DESC "Wifi ????"
  //#define CONFIG_MENU_ITEM_4_GCODE "M118 ????"
  //#define CONFIG_MENU_ITEM_4_CONFIRM

  //#define CONFIG_MENU_ITEM_5_DESC "Wifi ????"
  //#define CONFIG_MENU_ITEM_5_GCODE "M118 ????"
  //#define CONFIG_MENU_ITEM_5_CONFIRM
#endif


// @section host

/**
 * Host Action Commands
 *
 * Define host streamer action commands in compliance with the standard.
 *
 * See https://reprap.org/wiki/G-code#Action_commands
 * Common commands ........ poweroff, pause, paused, resume, resumed, cancel
 * G29_RETRY_AND_RECOVER .. probe_rewipe, probe_failed
 *
 * Some features add reason codes to extend these commands.
 *
 * Host Prompt Support enables Marlin to use the host for user prompts so
 * filament runout and other processes can be managed from the host side.
 */
#define HOST_ACTION_COMMANDS
#if ENABLED(HOST_ACTION_COMMANDS)
  #define HOST_PAUSE_M76                // Tell the host to pause in response to M76
  #define HOST_PROMPT_SUPPORT           // Initiate host prompts to get user feedback
  #if ENABLED(HOST_PROMPT_SUPPORT)
    #define HOST_STATUS_NOTIFICATIONS   // Send some status messages to the host as notifications
  #endif
  //#define HOST_START_MENU_ITEM          // Add a menu item that tells the host to start
  //#define HOST_SHUTDOWN_MENU_ITEM       // Add a menu item that tells the host to shut down
#endif

// @section extras

/**
 * Cancel Objects
 *
 * Implement M486 to allow Marlin to skip objects
 */
#define CANCEL_OBJECTS
#if ENABLED(CANCEL_OBJECTS)
  #define CANCEL_OBJECTS_REPORTING // Emit the current object as a status message
#endif

/**
 * Mechanical Gantry Calibration
 * Modern replacement for the Průša TMC_Z_CALIBRATION.
 * Adds capability to work with any adjustable current drivers.
 * Implemented as G34 because M915 is deprecated.
 * @section calibrate
 */
//#define MECHANICAL_GANTRY_CALIBRATION
#if ENABLED(MECHANICAL_GANTRY_CALIBRATION)
  #define GANTRY_CALIBRATION_CURRENT          600     // Default calibration current in ma
  #define GANTRY_CALIBRATION_EXTRA_HEIGHT      15     // Extra distance in mm past Z_###_POS to move
  #define GANTRY_CALIBRATION_FEEDRATE         500     // Feedrate for correction move
  //#define GANTRY_CALIBRATION_TO_MIN                 // Enable to calibrate Z in the MIN direction

  //#define GANTRY_CALIBRATION_SAFE_POSITION XY_CENTER // Safe position for nozzle
  //#define GANTRY_CALIBRATION_XY_PARK_FEEDRATE 3000  // XY Park Feedrate - MMM
  //#define GANTRY_CALIBRATION_COMMANDS_PRE   ""
  #define GANTRY_CALIBRATION_COMMANDS_POST  "G28"     // G28 highly recommended to ensure an accurate position
#endif

/**
 * Instant freeze / unfreeze functionality
 * Potentially useful for rapid stop that allows being resumed. Halts stepper movement.
 * Note this does NOT pause spindles, lasers, fans, heaters or any other auxiliary device.
 * @section interface
 */
//#define FREEZE_FEATURE
#if ENABLED(FREEZE_FEATURE)
  //#define FREEZE_PIN 41   // Override the default (KILL) pin here
  #define FREEZE_STATE LOW  // State of pin indicating freeze
#endif

/**
 * Advanced Print Counter settings
 * @section stats
 */
#if ENABLED(PRINTCOUNTER)
  #define SERVICE_WARNING_BUZZES  3
  // Activate up to 3 service interval watchdogs
  //#define SERVICE_NAME_1      "Service S"
  //#define SERVICE_INTERVAL_1  100 // print hours
  //#define SERVICE_NAME_2      "Service L"
  //#define SERVICE_INTERVAL_2  200 // print hours
  //#define SERVICE_NAME_3      "Service 3"
  //#define SERVICE_INTERVAL_3    1 // print hours
#endif

// @section develop

//
// M100 Free Memory Watcher to debug memory usage
//
//#define M100_FREE_MEMORY_WATCHER

//
// M42 - Set pin states
//
//#define DIRECT_PIN_CONTROL

//
// M43 - display pin status, toggle pins, watch pins, watch endstops & toggle LED, test servo probe
//
#define PINS_DEBUGGING

// Enable Tests that will run at startup and produce a report
//#define MARLIN_TEST_BUILD

// Enable Marlin dev mode which adds some special commands
//#define MARLIN_DEV_MODE

#if ENABLED(MARLIN_DEV_MODE)
  /**
   * D576 - Buffer Monitoring
   * To help diagnose print quality issues stemming from empty command buffers.
   */
  //#define BUFFER_MONITORING
#endif

/**
 * Postmortem Debugging captures misbehavior and outputs the CPU status and backtrace to serial.
 * When running in the debugger it will break for debugging. This is useful to help understand
 * a crash from a remote location. Requires ~400 bytes of SRAM and 5Kb of flash.
 */
//#define POSTMORTEM_DEBUGGING

/**
 * Software Reset options
 */
#define SOFT_RESET_VIA_SERIAL         // 'KILL' and '^X' commands will soft-reset the controller
//#define SOFT_RESET_ON_KILL            // Use a digital button to soft-reset the controller after KILL


// Shrink the build for smaller boards by sacrificing some serial feedback
#define MARLIN_SMALL_BUILD
