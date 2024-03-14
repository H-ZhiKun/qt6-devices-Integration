CREATE DATABASE integration;
-- Create the tables

-- Table structure for system_config
CREATE TABLE IF NOT EXISTS configs (
  id serial PRIMARY KEY,
  tag varchar(20) NOT NULL UNIQUE,
  config_json jsonb,
  username varchar(10) NOT NULL,
  created_time timestamp DEFAULT CURRENT_TIMESTAMP,
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP
);

-- Table structure for system_user
CREATE TABLE IF NOT EXISTS users (
  id serial PRIMARY KEY,
  username varchar(10) NOT NULL UNIQUE,
  password varchar(64) NOT NULL,
  camera_permission boolean,
  data_permission boolean,
  alarm_permission boolean,
  formula_permission boolean,
  debug_permission boolean,
  log_permission boolean,
  user_manage_permission boolean,
  auto_login boolean,
  created_time timestamp DEFAULT CURRENT_TIMESTAMP,
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP
);
INSERT INTO users (alarm_permission, auto_login, camera_permission, data_permission, formula_permission, log_permission, password, debug_permission, user_manage_permission, username) VALUES (true,true,true,true,true,true,'Il88AZH/C+ZHqBV1LEQ5Eg==
',true,true,'admin');

-- Table structure for everyday_data
CREATE TABLE IF NOT EXISTS everyday_data (
  id serial PRIMARY KEY,
  count_all integer,
  count_pass integer,
  count_waste integer,
  count_locate_waste integer,
  count_code_waste integer,
  count_pause_waste integer,
  batch_num varchar(256),
  formula_name varchar(128),
  alarm varchar(256),
  created_time timestamp DEFAULT CURRENT_TIMESTAMP,
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP
);

-- Table structure for elec_data
CREATE TABLE IF NOT EXISTS electric_data (
  id serial PRIMARY KEY,
  a_phase_voltage real,
  b_phase_voltage real,
  c_phase_voltage real,
  a_direction_current real,
  b_direction_current real,
  c_direction_current real,
  total_active_power real,
  total_apparent_power real,
  total_active_energy real,
  positive_active_energy real,
  reverse_active_energy real,
  combined_active_energy real,
  temperature real,
  humidity real,
  created_time timestamp DEFAULT CURRENT_TIMESTAMP
);

-- Table structure for camera_config
CREATE TABLE IF NOT EXISTS camera_config (
  id serial PRIMARY KEY,
  sn_num varchar(16) NOT NULL UNIQUE,
  qml_window smallint,
  max_width integer,
  max_height integer,
  fps integer,
  trigger_mode boolean,
  max_expose integer,
  expose integer,
  gain integer,
  width integer,
  height integer,
  offset_x integer,
  offset_y integer,
  created_time timestamp DEFAULT CURRENT_TIMESTAMP,
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP
);

-- Table structure for formula_data
CREATE TABLE IF NOT EXISTS formula_data (
  id serial PRIMARY KEY,
  name varchar(64) NOT NULL UNIQUE,
  code_x_position varchar(16),
  code_y_position varchar(16),
  impurity_locate varchar(16),
  speed_produce varchar(16),
  acceleration_produce varchar(16),
  deceleration_produce varchar(16),
  material_diameter varchar(16),
  bottle_type varchar(16),
  auto_load boolean DEFAULT FALSE,
  created_time timestamp DEFAULT CURRENT_TIMESTAMP,
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP
);

-- Table structure for operate_log
CREATE TABLE IF NOT EXISTS operate_log (
  id serial PRIMARY KEY,
  operate_name varchar(64),
  operate_page varchar(16),
  operate_function varchar(64),
  detail varchar(64),
  created_time timestamp DEFAULT CURRENT_TIMESTAMP,
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP
);

-- Table structure for alarm_data
CREATE TABLE IF NOT EXISTS alarm_data (
  id serial PRIMARY KEY,
  type varchar(16),
  content varchar(64),
  register_address varchar(10),
  state boolean DEFAULT TRUE,
  created_time timestamp DEFAULT CURRENT_TIMESTAMP,
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP
);

-- Table structure for plc_register_rw
CREATE TABLE IF NOT EXISTS plc_register_rw (
  id serial PRIMARY KEY,
  plc_address varchar(12) UNIQUE,
  value varchar(16),
  created_time timestamp DEFAULT CURRENT_TIMESTAMP,
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP
);

-- Table structure for circle_product_data
CREATE TABLE IF NOT EXISTS product_data(
   id serial,
   bottle_num VARCHAR(64),
   formula_name VARCHAR(64),
   is_remove BOOLEAN,
   is_ocr_equal_code BOOLEAN DEFAULT FALSE,
   qr_code VARCHAR(256),
   logistics VARCHAR(256),
   location_path VARCHAR(8),
   check_path VARCHAR(8),
   ocr_path VARCHAR(8),
   location_result VARCHAR(64),
   check_result VARCHAR(64),
   ocr_result VARCHAR(64),
   rotating_values VARCHAR(32),
   coding_values VARCHAR(32),
   remove_values VARCHAR(32),
   created_time timestamp(3) DEFAULT CURRENT_TIMESTAMP,
   PRIMARY KEY (id)
);
-- 添加索引
CREATE INDEX idx_check_result ON product_data (check_result);
CREATE INDEX idx_ocr_result ON product_data (ocr_result);
CREATE INDEX idx_logistics ON product_data (logistics);
CREATE INDEX idx_qr_code ON product_data (qr_code);
CREATE INDEX idx_location_path ON product_data (location_path);
CREATE INDEX idx_check_path ON product_data (check_path);
CREATE INDEX idx_ocr_path ON product_data (ocr_path);
CREATE INDEX idx_created_time ON product_data (created_time);
CREATE INDEX idx_is_ocr_equal_code ON product_data (is_ocr_equal_code);

-- Table structure for algorithm_config
CREATE TABLE IF NOT EXISTS algorithm_config(
  id serial,
  qrcode_angle VARCHAR(64),
  location_angle VARCHAR(64),
  location_check_angle VARCHAR(64),
  code_check_angle VARCHAR(64),
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP,
  created_time timestamp(3) DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (id)
);
INSERT INTO algorithm_config (qrcode_angle, location_angle, location_check_angle, code_check_angle)
VALUES('137', '185', '285', '193');

CREATE TABLE IF NOT EXISTS statistics(
  id serial,
  test_man VARCHAR(64),
  speed VARCHAR(12),
  bottom_num VARCHAR(12),
  bottom_distance VARCHAR(4),
  qrcode_data VARCHAR(128),
  print_data VARCHAR(128),
  ocr_data VARCHAR(128),
  rotate_stable VARCHAR(128),
  circle VARCHAR(128),
  star VARCHAR(128),
  tangle_data VARCHAR(128),
  check_data VARCHAR(128),
  remove_data VARCHAR(128),
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP,
  created_time timestamp(3) DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (id)
);
COMMENT ON COLUMN statistics.ocr_data IS 'If it is straight line, it represents OCR data; if it is  cap, it represents neissi data';