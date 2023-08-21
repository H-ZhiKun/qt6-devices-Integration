ALTER USER 'root'@'localhost' IDENTIFIED BY '123456';
CREATE DATABASE IF NOT EXISTS `integration` DEFAULT CHARSET utf8 COLLATE utf8_bin;
USE `integration`;
SET NAMES utf8;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for system_config
-- ----------------------------
CREATE TABLE IF NOT EXISTS  `configs`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `tag` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL UNIQUE,
  `config_json` json NULL,
  `username` varchar(10) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `created_time` datetime NULL DEFAULT CURRENT_TIMESTAMP UNIQUE,
  `updated_time` datetime NULL DEFAULT CURRENT_TIMESTAMP UNIQUE ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 7 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Table structure for system_op_logs
-- ----------------------------
CREATE TABLE IF NOT EXISTS `operation_logs`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `username` varchar(10) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `description` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `created_time` datetime NULL DEFAULT CURRENT_TIMESTAMP UNIQUE,
  `updated_time` datetime NULL DEFAULT CURRENT_TIMESTAMP UNIQUE ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = DYNAMIC;


-- ----------------------------
-- Table structure for system_user
-- ----------------------------
CREATE TABLE IF NOT EXISTS `users`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `username` varchar(10) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL UNIQUE,
  `password` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `permissions` json NULL,
  `created_time` datetime NULL DEFAULT CURRENT_TIMESTAMP UNIQUE,
  `updated_time` datetime NULL DEFAULT CURRENT_TIMESTAMP UNIQUE ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = DYNAMIC;


-- ----------------------------
-- Table structure for everyday_data
-- ----------------------------
CREATE TABLE IF NOT EXISTS `everyday_data`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `count_all` int UNSIGNED,
  `count_pass` int UNSIGNED,
  `count_waste` int UNSIGNED,
  `count_locate_waste` int UNSIGNED,
  `count_code_waste` int UNSIGNED,
  `count_pause_waste` int UNSIGNED,
  `batch_num` varchar(256) CHARACTER SET utf8 COLLATE utf8_bin,
  `formula_name` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin,
  `alarm` varchar(256) CHARACTER SET utf8 COLLATE utf8_bin,
  `created_time` datetime NULL DEFAULT CURRENT_TIMESTAMP UNIQUE,
  `updated_time` datetime NULL DEFAULT CURRENT_TIMESTAMP UNIQUE ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = DYNAMIC;


-- ----------------------------
-- Table structure for elec_data
-- ----------------------------
CREATE TABLE IF NOT EXISTS `electric_data`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `positive_active_energy` float,
  `reverse_active_energy` float,
	`a_phase_voltage` float,
	`b_phase_voltage` float,
	`c_phase_voltage` float,
	`temperature` float,
	`total_active_power` float,
	`total_apparent_power` float,
	`total_active_energy` float,
	`a_direction_current` float,
	`b_direction_current` float,
	`c_direction_current` float,
	`humidity` float,
  `created_time` datetime NULL DEFAULT CURRENT_TIMESTAMP UNIQUE,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Table structure for camera_config
-- ----------------------------
CREATE TABLE IF NOT EXISTS `camera_config`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `sn_num` varchar(16) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL UNIQUE,
  `qml_window` smallint,
	`max_width` int,
	`max_height` int,
	`fps` int,
	`trigger_mode` boolean,
  `max_expose` int,
	`expose` int,
	`gain` int,
	`width` int,
	`height` int,
	`offset_x` int,
	`offset_y` int,
  `created_time` datetime NULL DEFAULT CURRENT_TIMESTAMP UNIQUE,
  `updated_time` datetime NULL DEFAULT CURRENT_TIMESTAMP UNIQUE ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = DYNAMIC;


-- ----------------------------
-- Table structure for formula_data
-- ----------------------------
CREATE TABLE IF NOT EXISTS `formula_data`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `name` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin,
  `code_x_position` varchar(16),
  `code_y_position` varchar(16),
	`impurity_locate` varchar(16),
	`speed_produce` varchar(16),
	`acceleration_produce` varchar(16),
	`deceleration_produce` varchar(16),
	`created_time` datetime NULL DEFAULT CURRENT_TIMESTAMP UNIQUE,
  `updated_time` datetime NULL DEFAULT CURRENT_TIMESTAMP UNIQUE ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = DYNAMIC;


-- ----------------------------
-- Table structure for alarm_data
-- ----------------------------
CREATE TABLE IF NOT EXISTS `alarm_data`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `type` varchar(16) CHARACTER SET utf8 COLLATE utf8_bin,
  `content` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin,
	`register_address` varchar(10) CHARACTER SET utf8 COLLATE utf8_bin,
  `state` boolean DEFAULT TRUE,
	`created_time` datetime NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_time` datetime NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Table structure for plc_register_rw
-- ----------------------------
CREATE TABLE IF NOT EXISTS `plc_register_rw`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `plc_address` varchar(12) CHARACTER SET utf8 COLLATE utf8_bin UNIQUE,
  `value` varchar(16) CHARACTER SET utf8 COLLATE utf8_bin,
	`created_time` datetime NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_time` datetime NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = DYNAMIC;

SET FOREIGN_KEY_CHECKS = 1;
