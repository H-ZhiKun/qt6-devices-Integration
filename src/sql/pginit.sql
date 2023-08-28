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
