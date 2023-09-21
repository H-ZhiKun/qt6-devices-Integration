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

-- Table structure for system_op_logs
CREATE TABLE IF NOT EXISTS operation_logs (
  id serial PRIMARY KEY,
  username varchar(10) NOT NULL,
  description varchar(50) NOT NULL,
  created_time timestamp DEFAULT CURRENT_TIMESTAMP,
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP
);

-- Table structure for system_user
CREATE TABLE IF NOT EXISTS users (
  id serial PRIMARY KEY,
  username varchar(10) NOT NULL UNIQUE,
  password varchar(64) NOT NULL,
  permissions jsonb,
  created_time timestamp DEFAULT CURRENT_TIMESTAMP,
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP
);

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
  positive_active_energy real,
  reverse_active_energy real,
  a_phase_voltage real,
  b_phase_voltage real,
  c_phase_voltage real,
  temperature real,
  total_active_power real,
  total_apparent_power real,
  total_active_energy real,
  a_direction_current real,
  b_direction_current real,
  c_direction_current real,
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
  name varchar(64),
  code_x_position varchar(16),
  code_y_position varchar(16),
  impurity_locate varchar(16),
  speed_produce varchar(16),
  acceleration_produce varchar(16),
  deceleration_produce varchar(16),
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

-- Table structure for users
CREATE TABLE IF NOT EXISTS users  (
  "id" serial NOT NULL PRIMARY KEY,
  "name" varchar(32) NOT NULL UNIQUE,
  "password" varchar(32) NOT NULL,
	"camera_permission" smallint,
	"data_permission" smallint,
	"alarm_permission" smallint,
	"formula_permission" smallint,
	"sensor_permission" smallint,
	"valve_permission" smallint,
	"power_permission" smallint,
	"log_permission" smallint,
	"user_manage_permission" smallint,
  created_time timestamp DEFAULT CURRENT_TIMESTAMP,
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP
);

-- Table structure for bottles_record
CREATE TABLE IF NOT EXISTS bottles_record (
  id SERIAL PRIMARY KEY,
  batch_number VARCHAR(64),               -- 批号
  formula_name VARCHAR(64),               -- 配方名
  bottle_number VARCHAR(64),              -- 玻瓶号
  qrcode VARCHAR(256) NOT NULL,           -- 二维码读码结果
  logistics_code VARCHAR(64) NOT NULL,    -- 物流码
  locate_path VARCHAR(256) NOT NULL,      -- 定位图像路径
  locate_result VARCHAR(64) NOT NULL,     -- 定位结果路径
  check_path VARCHAR(256) NOT NULL,       -- 定位复核图像路径
  check_result VARCHAR(256) NOT NULL,     -- 定位复核结果路径
  printer_send VARCHAR(128) NOT NULL,     -- 打码机写入数据
  printer_result VARCHAR(32) NOT NULL,    -- 打码机返回结果
  ocr_path VARCHAR(256) NOT NULL,         -- 打码复核图像路径
  ocr_result VARCHAR(256) NOT NULL,       -- 打码复核结果路径
  is_complete BOOLEAN,                    -- 是否完成
  created_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS bottles_record (
  id SERIAL PRIMARY KEY,
  bottle_number VARCHAR(64),              -- 玻瓶号
  qrcode_signal_time TIMESTAMP,           -- 二维码读码工位信号
  qrcode_time TIMESTAMP,                  -- 二维码读码
  logistics_req_time TIMESTAMP,           -- 物流码请求时间
  logistics_ret_time TIMESTAMP,                  -- 物流码回复时间
  created_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Table structure for stright_alarm_data
CREATE TABLE IF NOT EXISTS stright_alarm_data (
  id serial PRIMARY KEY,
  type varchar(16),
  content varchar(64),
  register_address varchar(10),
  state boolean DEFAULT TRUE,
  created_time timestamp DEFAULT CURRENT_TIMESTAMP,
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP
);

-- Table structure for stright_count_data
CREATE TABLE IF NOT EXISTS stright_count_data(
  id serial PRIMARY KEY,
  count_in integer,
  count_detect integer,
  count_remove_all integer,
  count_remove_code integer,
  created_time timestamp DEFAULT CURRENT_TIMESTAMP,
  updated_time timestamp DEFAULT CURRENT_TIMESTAMP
);

-- Table structure for time_point_data
CREATE TABLE IF NOT EXISTS time_point_data(
   id serial NOT NULL,
   typePd_ VARCHAR(64),
   bottle_num VARCHAR(64),
   batch_num VARCHAR(64),
   formula_name VARCHAR(64),
   is_complete BOOLEAN,
   qrcode_signal_time timestamp,
   qrcode_time timestamp,
   logistics_ret_time timestamp,
   location_signal_time timestamp,
   location_image_time timestamp,
   location_result_time timestamp,
   check_signal_time timestamp,
   check_image_time timestamp,
   check_result_time timestamp,
   coding_signal_time timestamp,
   ocr_signal_time timestamp,
   ocr_image_time timestamp,
   ocr_result_time timestamp,
   complete_signal_time timestamp,
   created_time timestamp DEFAULT CURRENT_TIMESTAMP
)PARTITION BY RANGE (EXTRACT(YEAR_MONTH FROM created_time));
CREATE INDEX idx_created_time ON time_point_data (created_time);

-- Table structure for time_point_data
CREATE TABLE IF NOT EXISTS detect_data(
   id serial NOT NULL,
   typePd_ VARCHAR(64),
   bottle_num VARCHAR(64),
   batch_num VARCHAR(64),
   formula_name VARCHAR(64),
   is_complete BOOLEAN,
   qrcode_result VARCHAR(256),
   logistics_true_value_1 VARCHAR(256),
   logistics_true_value_2 VARCHAR(256),
   location_image_path VARCHAR(256),
   check_image_path VARCHAR(256),
   ocr_image_path VARCHAR(256),
   location_result VARCHAR(64),
   check_result VARCHAR(64),
   ocr_result VARCHAR(256),
   created_time timestamp DEFAULT CURRENT_TIMESTAMP
)PARTITION BY RANGE (EXTRACT(YEAR_MONTH FROM created_time));
CREATE INDEX idx_created_time ON time_point_data (created_time);

