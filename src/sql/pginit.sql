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

-- -- Table structure for line_product_data
-- CREATE TABLE IF NOT EXISTS line_product_data (
--   id serial PRIMARY KEY,
--   qr_code varchar(24),           --二维码读取结果
--   logistucs_code varchar(24),    --物流码获取结果
--   code_check_image varchar(128), --oce图片保存路径
--   code_check_res smallint,       --ocr返回的结果
--   ng_result smallint,                     --是否ng
--   created_time timestamp DEFAULT CURRENT_TIMESTAMP
-- );

-- -- Table structure for line_product_time
-- CREATE TABLE IF NOT EXISTS line_product_time (
--   id serial PRIMARY KEY,
--   enter_time varchar(24),            --入队/读码时间
--   require_qrcode_time varchar(24),   --二维码获取时间
--   require_logistics_time varchar(24), --物流码获取时间  
--   paint_time varchar(24),            --打码时间
--   ocr_photo_time varchar(24),        --ocr拍照时间
--   ocr_send_time varchar(24),         --ocr图片发送时间
--   ocr_reslut_time varchar(24),       --ocr结果获取时间
--   out_time varchar(24),              --剔除时间
--   created_time timestamp DEFAULT CURRENT_TIMESTAMP
-- );

-- -- Table structure for line_product_data
-- CREATE TABLE IF NOT EXISTS circle_product_data (
--   id serial PRIMARY KEY,
--   qr_code varchar(24),             --二维码读取结果
--   logistics_code varchar(24),      --物流码获取结果
--   locate_image varchar(128),       --定位图片路径
--   locate_res smallint,             --定位结果
--   locate_check_image varchar(128), --定位复合图片路径
--   locate_check_res smallint,       --定位复合结果
--   code_check_image varchar(128),   --oce图片保存路径
--   code_check_res smallint,         --ocr返回的结果
--   created_time timestamp DEFAULT CURRENT_TIMESTAMP
-- );

-- -- Table structure for line_product_time
-- CREATE TABLE IF NOT EXISTS circle_product_time (
--   id serial PRIMARY KEY,
--   enter_time varchar(24),               --入队时间
--   require_qrcode_time varchar(24),      --二维码获取时间
--   require_logistics_time varchar(24),   --物流码获取时间 
--   locate_photo_time varchar(24),        --定位拍照时间
--   locate_send_time varchar(24),         --定位图片发送时间
--   locate_reslut_time varchar(24),       --定位结果获取时间
--   locate_check_photo_time varchar(24),  --定位复合拍照时间
--   locate_check_send_time varchar(24),   --定位复合图片发送时间
--   locate_check_reslut_time varchar(24), --定位复合结果获取时间
--   paint_singnal_time varchar(24),       --打码信号获取时间
--   paint_time varchar(24),               --打码时间
--   ocr_photo_time varchar(24),           --ocr拍照时间
--   ocr_send_time varchar(24),            --ocr图片发送时间
--   ocr_reslut_time varchar(24),          --ocr结果获取时间
--   out_time varchar(24),                 --剔除时间
--   created_time timestamp DEFAULT CURRENT_TIMESTAMP
-- );

-- Table structure for product_time
CREATE TABLE IF NOT EXISTS product_time(
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
   created_time timestamp DEFAULT CURRENT_TIMESTAMP,
   created_date date DEFAULT CURRENT_DATE
)PARTITION BY RANGE (created_date);
CREATE INDEX idx_product_time ON product_time (created_time);

-- Table structure for product_data
CREATE TABLE IF NOT EXISTS product_data(
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
   created_time timestamp DEFAULT CURRENT_TIMESTAMP,
   created_date date DEFAULT CURRENT_DATE
)PARTITION BY RANGE (created_date);
CREATE INDEX idx_product_data ON product_data (created_time);


-- 创建分区表的函数
-- CREATE OR REPLACE FUNCTION create_partition_table()
-- RETURNS TRIGGER AS $$
-- BEGIN
--     DECLARE
--         partition_table_name TEXT := 'product_time_' || to_char(CURRENT_DATE, 'YYYY_MM');
--     BEGIN
--         IF NOT EXISTS (
--             SELECT 1
--             FROM pg_tables
--             WHERE tablename = partition_table_name
--         ) THEN
--             EXECUTE 'CREATE TABLE ' || partition_table_name || ' PARTITION OF product_time FOR VALUES FROM (''' || to_char(CURRENT_DATE, 'YYYY-MM-01') || ''') TO (''' || to_char((CURRENT_DATE + INTERVAL '1 month')::date, 'YYYY-MM-01') || ''')';
--         END IF;
--         EXECUTE 'INSERT INTO ' || partition_table_name || ' SELECT ($1).*'
--         USING NEW;    
--         RETURN NULL;
--     END;
-- END;
-- $$ LANGUAGE plpgsql;

-- CREATE TRIGGER insert_partition_trigger
-- BEFORE INSERT ON "public"."product_time"
-- FOR EACH STATEMENT
-- EXECUTE PROCEDURE "public"."create_partition_table"();





