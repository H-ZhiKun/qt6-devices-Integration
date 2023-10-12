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
  camera_permission boolean,
  data_permission boolean,
  alarm_permission boolean,
  formula_permission boolean,
  sensor_permission boolean,
  valve_permission boolean,
  power_permission boolean,
  log_permission boolean,
  user_manage_permission boolean,
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

-- Table structure for line_product_time
CREATE TABLE IF NOT EXISTS line_product_time(
   id serial,
   type_pd VARCHAR(64),
   bottle_num VARCHAR(64),
   batch_num VARCHAR(64),
   formula_name VARCHAR(64),
   is_remove BOOLEAN,
   qrcode_time timestamp(3),
   logistics_ret_time timestamp(3),
   coding_signal_time timestamp(3),
   ocr_signal_time timestamp(3),
   ocr_image_time timestamp(3),
   ocr_result_time timestamp(3),
   complete_signal_time timestamp(3),
   created_time timestamp(3) DEFAULT CURRENT_TIMESTAMP,
   created_date date DEFAULT CURRENT_DATE,
   PRIMARY KEY (id, created_date)
)PARTITION BY RANGE (created_date);
CREATE INDEX idx_line_product_time ON line_product_time (created_time);

-- Table structure for line_product_data
CREATE TABLE IF NOT EXISTS line_product_data(
   id serial,
   type_pd VARCHAR(64),
   bottle_num VARCHAR(64),
   batch_num VARCHAR(64),
   formula_name VARCHAR(64),
   is_remove BOOLEAN,
   qrcode_result VARCHAR(256),
   logistics_true_value_1 VARCHAR(256),
   logistics_true_value_2 VARCHAR(256),
   ocr_image_path VARCHAR(256),
   ocr_result VARCHAR(256),
   created_time timestamp(3) DEFAULT CURRENT_TIMESTAMP,
   created_date date DEFAULT CURRENT_DATE,
   PRIMARY KEY (id, created_date)
)PARTITION BY RANGE (created_date);
CREATE INDEX idx_line_product_data ON line_product_data (created_time);

CREATE TABLE line_product_time_2023_09 PARTITION OF line_product_time FOR VALUES FROM ('2023-09-01') TO ('2023-10-01');
CREATE TABLE line_product_data_2023_09 PARTITION OF line_product_data FOR VALUES FROM ('2023-09-01') TO ('2023-10-01');
CREATE TABLE line_product_time_2023_10 PARTITION OF line_product_time FOR VALUES FROM ('2023-10-01') TO ('2023-11-01');
CREATE TABLE line_product_data_2023_10 PARTITION OF line_product_data FOR VALUES FROM ('2023-10-01') TO ('2023-11-01');
CREATE TABLE line_product_time_2023_11 PARTITION OF line_product_time FOR VALUES FROM ('2023-11-01') TO ('2023-12-01');
CREATE TABLE line_product_data_2023_11 PARTITION OF line_product_data FOR VALUES FROM ('2023-11-01') TO ('2023-12-01');
CREATE TABLE line_product_time_2023_12 PARTITION OF line_product_time FOR VALUES FROM ('2023-12-01') TO ('2024-01-01');
CREATE TABLE line_product_data_2023_12 PARTITION OF line_product_data FOR VALUES FROM ('2023-12-01') TO ('2024-01-01');

-- Table structure for circle_product_time
CREATE TABLE IF NOT EXISTS circle_product_time(
   id serial,
   type_pd VARCHAR(64),
   bottle_num VARCHAR(64),
   batch_num VARCHAR(64),
   formula_name VARCHAR(64),
   is_remove BOOLEAN,
   qrcode_time timestamp(3),
   qrcode_signal_time timestamp(3),
   logistics_ret_time timestamp(3),
   issued_rotate_time timestamp(3),
   location_signal_time timestamp(3),
   location_image_time timestamp(3),
   location_result_time timestamp(3),
   issued_locateCheck_time timestamp(3),
   check_signal_time timestamp(3),
   check_image_time timestamp(3),
   check_result_time timestamp(3),
   coding_signal_time timestamp(3),
   ocr_signal_time timestamp(3),
   ocr_image_time timestamp(3),
   ocr_result_time timestamp(3),
   remove_signal_time timestamp(3),
   created_time timestamp(3) DEFAULT CURRENT_TIMESTAMP,
   created_date date DEFAULT CURRENT_DATE,
   PRIMARY KEY (id, created_date)
)PARTITION BY RANGE (created_date);
CREATE INDEX idx_circle_product_time ON circle_product_time (created_time);

-- Table structure for circle_product_data
CREATE TABLE IF NOT EXISTS circle_product_data(
   id serial,
   type_pd VARCHAR(64),
   bottle_num VARCHAR(64),
   batch_num VARCHAR(64),
   formula_name VARCHAR(64),
   is_remove BOOLEAN,
   qrcode_result VARCHAR(256),
   logistics_true_value_1 VARCHAR(256),
   logistics_true_value_2 VARCHAR(256),
   location_image_path VARCHAR(256),
   check_image_path VARCHAR(256),
   ocr_image_path VARCHAR(256),
   location_result VARCHAR(64),
   check_result VARCHAR(64),
   ocr_result VARCHAR(256),
   created_time timestamp(3) DEFAULT CURRENT_TIMESTAMP,
   created_date date DEFAULT CURRENT_DATE,
   PRIMARY KEY (id, created_date)
)PARTITION BY RANGE (created_date);
CREATE INDEX idx_circle_product_data ON circle_product_data (created_time);

CREATE TABLE circle_product_time_2023_09 PARTITION OF circle_product_time FOR VALUES FROM ('2023-09-01') TO ('2023-10-01');
CREATE TABLE circle_product_data_2023_09 PARTITION OF circle_product_data FOR VALUES FROM ('2023-09-01') TO ('2023-10-01');
CREATE TABLE circle_product_time_2023_10 PARTITION OF circle_product_time FOR VALUES FROM ('2023-10-01') TO ('2023-11-01');
CREATE TABLE circle_product_data_2023_10 PARTITION OF circle_product_data FOR VALUES FROM ('2023-10-01') TO ('2023-11-01');
CREATE TABLE circle_product_time_2023_11 PARTITION OF circle_product_time FOR VALUES FROM ('2023-11-01') TO ('2023-12-01');
CREATE TABLE circle_product_data_2023_11 PARTITION OF circle_product_data FOR VALUES FROM ('2023-11-01') TO ('2023-12-01');
CREATE TABLE circle_product_time_2023_12 PARTITION OF circle_product_time FOR VALUES FROM ('2023-12-01') TO ('2024-01-01');
CREATE TABLE circle_product_data_2023_12 PARTITION OF circle_product_data FOR VALUES FROM ('2023-12-01') TO ('2024-01-01');


-- Table structure for cap_product_time
CREATE TABLE IF NOT EXISTS cap_product_time(
   id serial,
   type_pd VARCHAR(64),
   bottle_num VARCHAR(64),
   batch_num VARCHAR(64),
   formula_name VARCHAR(64),
   is_remove BOOLEAN,
   qrcode_time timestamp(3),
   logistics_ret_time timestamp(3),
   ocr_signal_time timestamp(3),
   ocr_image_time timestamp(3),
   ocr_result_time timestamp(3),
   complete_signal_time timestamp(3),
   created_time timestamp(3) DEFAULT CURRENT_TIMESTAMP,
   created_date date DEFAULT CURRENT_DATE,
   PRIMARY KEY (id, created_date)
)PARTITION BY RANGE (created_date);
CREATE INDEX idx_cap_product_time ON cap_product_time (created_time);

-- Table structure for line_product_data
CREATE TABLE IF NOT EXISTS cap_product_data(
   id serial,
   type_pd VARCHAR(64),
   bottle_num VARCHAR(64),
   batch_num VARCHAR(64),
   formula_name VARCHAR(64),
   is_remove BOOLEAN,
   qrcode_result VARCHAR(256),
   logistics_true_value_1 VARCHAR(256),
   logistics_true_value_2 VARCHAR(256),
   ocr_image_path VARCHAR(256),
   ocr_result VARCHAR(256),
   created_time timestamp(3) DEFAULT CURRENT_TIMESTAMP,
   created_date date DEFAULT CURRENT_DATE,
   PRIMARY KEY (id, created_date)
)PARTITION BY RANGE (created_date);
CREATE INDEX idx_cap_product_data ON cap_product_data (created_time);

CREATE TABLE cap_product_time_2023_09 PARTITION OF cap_product_time FOR VALUES FROM ('2023-09-01') TO ('2023-10-01');
CREATE TABLE cap_product_data_2023_09 PARTITION OF cap_product_data FOR VALUES FROM ('2023-09-01') TO ('2023-10-01');
CREATE TABLE cap_product_time_2023_10 PARTITION OF cap_product_time FOR VALUES FROM ('2023-10-01') TO ('2023-11-01');
CREATE TABLE cap_product_data_2023_10 PARTITION OF cap_product_data FOR VALUES FROM ('2023-10-01') TO ('2023-11-01');
CREATE TABLE cap_product_time_2023_11 PARTITION OF cap_product_time FOR VALUES FROM ('2023-11-01') TO ('2023-12-01');
CREATE TABLE cap_product_data_2023_11 PARTITION OF cap_product_data FOR VALUES FROM ('2023-11-01') TO ('2023-12-01');
CREATE TABLE cap_product_time_2023_12 PARTITION OF cap_product_time FOR VALUES FROM ('2023-12-01') TO ('2024-01-01');
CREATE TABLE cap_product_data_2023_12 PARTITION OF cap_product_data FOR VALUES FROM ('2023-12-01') TO ('2024-01-01');


--分区表创建函数
-- CREATE OR REPLACE FUNCTION create_partition()
-- RETURNS VOID AS $$
-- DECLARE
--     partition_date DATE := date_trunc('month', CURRENT_DATE)::DATE;
--     circle_product_time_table_name TEXT := format('circle_product_time_%s_%s', EXTRACT(YEAR FROM partition_date)::TEXT, LPAD(EXTRACT(MONTH FROM partition_date)::TEXT, 2));
--     circle_product_data_table_name TEXT := format('circle_product_data_%s_%s', EXTRACT(YEAR FROM partition_date)::TEXT, LPAD(EXTRACT(MONTH FROM partition_date)::TEXT, 2));
--     line_product_time_table_name TEXT := format('line_product_time_%s_%s', EXTRACT(YEAR FROM partition_date)::TEXT, LPAD(EXTRACT(MONTH FROM partition_date)::TEXT, 2));
--     line_product_data_table_name TEXT := format('line_product_data_%s_%s', EXTRACT(YEAR FROM partition_date)::TEXT, LPAD(EXTRACT(MONTH FROM partition_date)::TEXT, 2));
-- BEGIN
--     EXECUTE format(
--         'CREATE TABLE IF NOT EXISTS %I PARTITION OF circle_product_time FOR VALUES FROM (%L) TO (%L)',
--         circle_product_time_table_name,
--         partition_date,
--         (partition_date + INTERVAL '1 month')::DATE
--     );
--     EXECUTE format(
--         'CREATE TABLE IF NOT EXISTS %I PARTITION OF circle_product_data FOR VALUES FROM (%L) TO (%L)',
--         circle_product_data_table_name,
--         partition_date,
--         (partition_date + INTERVAL '1 month')::DATE
--     );
--     EXECUTE format(
--         'CREATE TABLE IF NOT EXISTS %I PARTITION OF line_product_time FOR VALUES FROM (%L) TO (%L)',
--         line_product_time_table_name,
--         partition_date,
--         (partition_date + INTERVAL '1 month')::DATE
--     );
--     EXECUTE format(
--         'CREATE TABLE IF NOT EXISTS %I PARTITION OF line_product_data FOR VALUES FROM (%L) TO (%L)',
--         line_product_data_table_name,
--         partition_date,
--         (partition_date + INTERVAL '1 month')::DATE
--     );
-- END;
-- $$ LANGUAGE plpgsql;

-- --创建pgagent定时job，时间间隔1分钟执行
-- DO $$
-- DECLARE
--     jid integer;
--     scid integer;
-- BEGIN
-- -- Creating a new job
-- INSERT INTO pgagent.pga_job(
--     jobjclid, jobname, jobdesc, jobhostagent, jobenabled
-- ) VALUES (
--     1::integer, 'pgagent_'::text, ''::text, ''::text, true
-- ) RETURNING jobid INTO jid;

-- -- Steps
-- -- Inserting a step (jobid: NULL)
-- INSERT INTO pgagent.pga_jobstep (
--     jstjobid, jstname, jstenabled, jstkind,
--     jstconnstr, jstdbname, jstonerror,
--     jstcode, jstdesc
-- ) VALUES (
--     jid, 'step1'::text, true, 's'::character(1),
--     ''::text, 'integration'::name, 'i'::character(1),
--     'SELECT create_partition();'::text, ''::text
-- ) ;

-- -- Schedules
-- -- Inserting a schedule
-- INSERT INTO pgagent.pga_schedule(
--     jscjobid, jscname, jscdesc, jscenabled,
--     jscstart, jscend,    jscminutes, jschours, jscweekdays, jscmonthdays, jscmonths
-- ) VALUES (
--     jid, 'schedule'::text, ''::text, true,
--     '2023-10-08 12:43:39 +08:00'::timestamp with time zone, '2099-11-26T12:43:40+08:00'::timestamp with time zone,
--     -- Minutes
--     ARRAY[true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true]::boolean[],
--     -- Hours
--     ARRAY[true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true]::boolean[],
--     -- Week days
--     ARRAY[true,true,true,true,true,true,true]::boolean[],
--     -- Month days
--     ARRAY[true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true]::boolean[],
--     -- Months
--     ARRAY[true,true,true,true,true,true,true,true,true,true,true,true]::boolean[]
-- ) RETURNING jscid INTO scid;
-- END
-- $$;




