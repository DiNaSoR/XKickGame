CREATE DATABASE IF NOT EXISTS xkick CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
CREATE DATABASE IF NOT EXISTS xkick_log CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
CREATE DATABASE IF NOT EXISTS xkick_sample CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;

USE xkick;

CREATE TABLE IF NOT EXISTS tb_game_server (
  server_code INT NOT NULL PRIMARY KEY,
  server_channel INT NOT NULL DEFAULT 1,
  server_state CHAR(1) NOT NULL DEFAULT '1',
  server_name VARCHAR(64) NOT NULL DEFAULT '',
  server_match INT NOT NULL DEFAULT 0,
  server_job INT NOT NULL DEFAULT 0,
  server_free INT NOT NULL DEFAULT 1,
  server_slevel INT NOT NULL DEFAULT 1,
  server_elevel INT NOT NULL DEFAULT 99,
  server_max INT NOT NULL DEFAULT 100,
  server_current INT NOT NULL DEFAULT 0,
  server_inip VARCHAR(64) NOT NULL DEFAULT '127.0.0.1',
  server_exip VARCHAR(64) NOT NULL DEFAULT '127.0.0.1',
  server_port INT NOT NULL DEFAULT 2001
);

INSERT INTO tb_game_server (
  server_code, server_channel, server_state, server_name, server_match, server_job,
  server_free, server_slevel, server_elevel, server_max, server_current,
  server_inip, server_exip, server_port
) VALUES
  (0, 0, '1', 'Local bootstrap', 0, 0, 1, 1, 99, 100, 0, '127.0.0.1', '127.0.0.1', 2001),
  (1, 0, '1', 'Certify', 0, 0, 1, 1, 99, 100, 0, '127.0.0.1', '127.0.0.1', 2001),
  (100, 1, '2', 'Local Game', 0, 0, 1, 1, 99, 100, 0, '127.0.0.1', '127.0.0.1', 3001),
  (101, 1, '2', 'Game 1', 0, 0, 1, 1, 99, 100, 0, '127.0.0.1', '127.0.0.1', 3001)
ON DUPLICATE KEY UPDATE
  server_channel = VALUES(server_channel),
  server_state = VALUES(server_state),
  server_name = VALUES(server_name),
  server_match = VALUES(server_match),
  server_job = VALUES(server_job),
  server_free = VALUES(server_free),
  server_slevel = VALUES(server_slevel),
  server_elevel = VALUES(server_elevel),
  server_max = VALUES(server_max),
  server_inip = VALUES(server_inip),
  server_exip = VALUES(server_exip),
  server_port = VALUES(server_port);

CREATE TABLE IF NOT EXISTS tb_mst_start (
  start_yn CHAR(1) NOT NULL DEFAULT 'Y',
  start_adminid VARCHAR(64) NOT NULL DEFAULT 'local',
  start_adminip VARCHAR(64) NOT NULL DEFAULT '127.0.0.1'
);

INSERT INTO tb_mst_start (start_yn, start_adminid, start_adminip)
SELECT 'Y', 'local', '127.0.0.1'
WHERE NOT EXISTS (SELECT 1 FROM tb_mst_start);

CREATE TABLE IF NOT EXISTS tb_game_notice (
  notice_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  notice_text VARCHAR(255) NOT NULL DEFAULT '',
  notice_date DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  notice_enable TINYINT NOT NULL DEFAULT 1
);

INSERT INTO tb_game_notice (notice_text, notice_date, notice_enable)
SELECT 'Welcome to the local XKick server', NOW(), 1
WHERE NOT EXISTS (SELECT 1 FROM tb_game_notice);

CREATE TABLE IF NOT EXISTS tb_game_weather (
  weather_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  weather_type INT NOT NULL DEFAULT 0,
  weather_start TIME NOT NULL DEFAULT '00:00:00',
  weather_end TIME NOT NULL DEFAULT '23:59:59',
  weather_date INT NOT NULL DEFAULT 0
);

INSERT INTO tb_game_weather (weather_type, weather_start, weather_end, weather_date)
SELECT 0, '00:00:00', '23:59:59', 0
WHERE NOT EXISTS (SELECT 1 FROM tb_game_weather);

CREATE TABLE IF NOT EXISTS tb_game_event (
  event_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  event_server INT NOT NULL DEFAULT 0,
  event_type INT NOT NULL DEFAULT 0,
  reward_type INT NOT NULL DEFAULT 0,
  reward_value INT NOT NULL DEFAULT 0,
  event_startdate DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  event_enddate DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS tb_mst_member (
  member_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  member_id VARCHAR(64) NOT NULL UNIQUE,
  member_pass VARCHAR(128) NOT NULL DEFAULT '',
  member_name VARCHAR(64) NOT NULL DEFAULT '',
  member_state INT NOT NULL DEFAULT 1,
  member_register VARCHAR(32) NOT NULL DEFAULT '0000000',
  member_partner VARCHAR(64) NOT NULL DEFAULT 'enterplay',
  member_ip VARCHAR(64) NOT NULL DEFAULT '',
  member_speed INT NOT NULL DEFAULT 0,
  member_createdate VARCHAR(32) NOT NULL DEFAULT '',
  member_logindate VARCHAR(32) NOT NULL DEFAULT '',
  member_deletedate VARCHAR(32) NOT NULL DEFAULT '',
  member_blockdate VARCHAR(32) NOT NULL DEFAULT '',
  member_releasedate VARCHAR(32) NOT NULL DEFAULT ''
);

CREATE TABLE IF NOT EXISTS tb_mst_meminfo (
  meminfo_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  member_register VARCHAR(32) NOT NULL DEFAULT '',
  meminfo_memo TEXT
);

CREATE TABLE IF NOT EXISTS tb_game_trio (
  member_seq INT NOT NULL PRIMARY KEY,
  trio_cash INT NOT NULL DEFAULT 0,
  trio_point INT NOT NULL DEFAULT 0,
  trio_credit INT NOT NULL DEFAULT 0,
  trio_allpoint INT NOT NULL DEFAULT 0,
  trio_allcredit INT NOT NULL DEFAULT 0,
  trio_server INT NOT NULL DEFAULT 0,
  trio_lastseq INT NOT NULL DEFAULT 0,
  trio_host INT NOT NULL DEFAULT 0,
  trio_count INT NOT NULL DEFAULT 3,
  trio_tutorial INT NOT NULL DEFAULT 0,
  trio_quest INT NOT NULL DEFAULT 0,
  trio_camera INT NOT NULL DEFAULT 12,
  trio_camerateam INT NOT NULL DEFAULT 1,
  trio_radian INT NOT NULL DEFAULT 0,
  trio_zoom INT NOT NULL DEFAULT 2,
  trio_target INT NOT NULL DEFAULT 10,
  trio_shadow INT NOT NULL DEFAULT 0,
  trio_label INT NOT NULL DEFAULT 0,
  trio_sound INT NOT NULL DEFAULT 8,
  trio_music INT NOT NULL DEFAULT 16,
  trio_invite INT NOT NULL DEFAULT 1,
  trio_whisper INT NOT NULL DEFAULT 1,
  trio_friend INT NOT NULL DEFAULT 1,
  trio_up INT NOT NULL DEFAULT 222,
  trio_down INT NOT NULL DEFAULT 230,
  trio_left INT NOT NULL DEFAULT 225,
  trio_right INT NOT NULL DEFAULT 227,
  trio_leftshoot INT NOT NULL DEFAULT 32,
  trio_rightshoot INT NOT NULL DEFAULT 33,
  trio_longpass INT NOT NULL DEFAULT 30,
  trio_shortpass INT NOT NULL DEFAULT 31,
  trio_screen INT NOT NULL DEFAULT 17,
  trio_tackle INT NOT NULL DEFAULT 16,
  trio_steal INT NOT NULL DEFAULT 18,
  trio_quick INT NOT NULL DEFAULT 42,
  trio_skill1 INT NOT NULL DEFAULT 46,
  trio_skill2 INT NOT NULL DEFAULT 45,
  trio_skill3 INT NOT NULL DEFAULT 47,
  trio_skill4 INT NOT NULL DEFAULT 57,
  trio_skill5 INT NOT NULL DEFAULT 0,
  trio_skillcode1 INT NOT NULL DEFAULT 0,
  trio_skillcode2 INT NOT NULL DEFAULT 0,
  trio_skillcode3 INT NOT NULL DEFAULT 0,
  trio_skillcode4 INT NOT NULL DEFAULT 0,
  trio_version VARCHAR(64) NOT NULL DEFAULT '',
  trio_logindate VARCHAR(32) NOT NULL DEFAULT '',
  trio_deletedate VARCHAR(32) NOT NULL DEFAULT ''
);

CREATE TABLE IF NOT EXISTS tb_game_player (
  player_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  member_seq INT NOT NULL DEFAULT 0,
  player_order INT NOT NULL DEFAULT 0,
  player_name VARCHAR(64) NOT NULL DEFAULT '',
  player_state CHAR(1) NOT NULL DEFAULT '1',
  player_gender INT NOT NULL DEFAULT 1,
  player_skin INT NOT NULL DEFAULT 11,
  player_uniform INT NOT NULL DEFAULT 0,
  player_face INT NOT NULL DEFAULT 101101111,
  player_hair INT NOT NULL DEFAULT 102101101,
  player_shirts INT NOT NULL DEFAULT 201001101,
  player_pants INT NOT NULL DEFAULT 202001101,
  player_shoes INT NOT NULL DEFAULT 204001101,
  player_level INT NOT NULL DEFAULT 1,
  player_exp INT NOT NULL DEFAULT 0,
  player_faculty INT NOT NULL DEFAULT 2,
  player_skill INT NOT NULL DEFAULT 100,
  player_position INT NOT NULL DEFAULT 10,
  player_condition INT NOT NULL DEFAULT 0,
  player_alias INT NOT NULL DEFAULT 0,
  player_operation INT NOT NULL DEFAULT 0,
  player_ment VARCHAR(255) NOT NULL DEFAULT '',
  player_rand INT NOT NULL DEFAULT 0,
  player_shopstate INT NOT NULL DEFAULT 0,
  player_cardentry INT NOT NULL DEFAULT 0,
  player_createdate VARCHAR(32) NOT NULL DEFAULT '',
  player_logindate VARCHAR(32) NOT NULL DEFAULT '',
  player_deletedate VARCHAR(32) NOT NULL DEFAULT '',
  player_shutupdate VARCHAR(32) NOT NULL DEFAULT ''
);

SET @missing_idx_tb_game_player_member = (
  SELECT COUNT(*) = 0
  FROM information_schema.statistics
  WHERE table_schema = DATABASE()
    AND table_name = 'tb_game_player'
    AND index_name = 'idx_tb_game_player_member'
);
SET @idx_tb_game_player_member_ddl = IF(
  @missing_idx_tb_game_player_member,
  'CREATE INDEX idx_tb_game_player_member ON tb_game_player (member_seq, player_state, player_order)',
  'SELECT 1'
);
PREPARE idx_tb_game_player_member_stmt FROM @idx_tb_game_player_member_ddl;
EXECUTE idx_tb_game_player_member_stmt;
DEALLOCATE PREPARE idx_tb_game_player_member_stmt;

SET @missing_idx_tb_game_player_name = (
  SELECT COUNT(*) = 0
  FROM information_schema.statistics
  WHERE table_schema = DATABASE()
    AND table_name = 'tb_game_player'
    AND index_name = 'idx_tb_game_player_name'
);
SET @idx_tb_game_player_name_ddl = IF(
  @missing_idx_tb_game_player_name,
  'CREATE INDEX idx_tb_game_player_name ON tb_game_player (player_name)',
  'SELECT 1'
);
PREPARE idx_tb_game_player_name_stmt FROM @idx_tb_game_player_name_ddl;
EXECUTE idx_tb_game_player_name_stmt;
DEALLOCATE PREPARE idx_tb_game_player_name_stmt;

CREATE TABLE IF NOT EXISTS tb_game_setting (
  player_seq INT NOT NULL PRIMARY KEY,
  setting_camera INT NOT NULL DEFAULT 0,
  setting_zoom INT NOT NULL DEFAULT 0,
  setting_target INT NOT NULL DEFAULT 0,
  setting_camerateam INT NOT NULL DEFAULT 0,
  setting_radian INT NOT NULL DEFAULT 0,
  setting_shadow INT NOT NULL DEFAULT 1,
  setting_label INT NOT NULL DEFAULT 1,
  setting_sound INT NOT NULL DEFAULT 1,
  setting_music INT NOT NULL DEFAULT 1,
  setting_invite INT NOT NULL DEFAULT 1,
  setting_whisper INT NOT NULL DEFAULT 1,
  setting_friend INT NOT NULL DEFAULT 1,
  setting_up INT NOT NULL DEFAULT 0,
  setting_down INT NOT NULL DEFAULT 0,
  setting_left INT NOT NULL DEFAULT 0,
  setting_right INT NOT NULL DEFAULT 0,
  setting_leftshoot INT NOT NULL DEFAULT 0,
  setting_rightshoot INT NOT NULL DEFAULT 0,
  setting_longpass INT NOT NULL DEFAULT 0,
  setting_shortpass INT NOT NULL DEFAULT 0,
  setting_screen INT NOT NULL DEFAULT 0,
  setting_tackle INT NOT NULL DEFAULT 0,
  setting_steal INT NOT NULL DEFAULT 0,
  setting_quick INT NOT NULL DEFAULT 0,
  setting_quick2 INT NOT NULL DEFAULT 0,
  setting_skill1 INT NOT NULL DEFAULT 0,
  setting_skill2 INT NOT NULL DEFAULT 0,
  setting_skill3 INT NOT NULL DEFAULT 0,
  setting_skill4 INT NOT NULL DEFAULT 0,
  setting_skill5 INT NOT NULL DEFAULT 0,
  setting_skill_attack1 INT NOT NULL DEFAULT 0,
  setting_skill_attack2 INT NOT NULL DEFAULT 0,
  setting_skill_attack3 INT NOT NULL DEFAULT 0,
  setting_skill_attack4 INT NOT NULL DEFAULT 0,
  setting_skill_attack5 INT NOT NULL DEFAULT 0,
  setting_skill_defence1 INT NOT NULL DEFAULT 0,
  setting_skill_defence2 INT NOT NULL DEFAULT 0,
  setting_skill_defence3 INT NOT NULL DEFAULT 0,
  setting_skill_defence4 INT NOT NULL DEFAULT 0,
  setting_skill_defence5 INT NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS tb_game_faculty (
  player_seq INT NOT NULL PRIMARY KEY,
  faculty_run INT NOT NULL DEFAULT 0,
  faculty_dribble INT NOT NULL DEFAULT 0,
  faculty_quick INT NOT NULL DEFAULT 0,
  faculty_stamina INT NOT NULL DEFAULT 0,
  faculty_elasticity INT NOT NULL DEFAULT 0,
  faculty_bodycheck INT NOT NULL DEFAULT 0,
  faculty_cross INT NOT NULL DEFAULT 0,
  faculty_shortpass INT NOT NULL DEFAULT 0,
  faculty_longpass INT NOT NULL DEFAULT 0,
  faculty_headshoot INT NOT NULL DEFAULT 0,
  faculty_shortshoot INT NOT NULL DEFAULT 0,
  faculty_longshoot INT NOT NULL DEFAULT 0,
  faculty_keeping INT NOT NULL DEFAULT 0,
  faculty_steal INT NOT NULL DEFAULT 0,
  faculty_tackle INT NOT NULL DEFAULT 0,
  faculty_catch INT NOT NULL DEFAULT 0,
  faculty_punch INT NOT NULL DEFAULT 0,
  faculty_guard INT NOT NULL DEFAULT 0,
  faculty_move INT NOT NULL DEFAULT 0,
  faculty_body INT NOT NULL DEFAULT 0,
  faculty_pass INT NOT NULL DEFAULT 0,
  faculty_shoot INT NOT NULL DEFAULT 0,
  faculty_defense INT NOT NULL DEFAULT 0,
  faculty_goalkeep INT NOT NULL DEFAULT 0,
  faculty_talent INT NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS tb_game_item (
  item_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  member_seq INT NOT NULL DEFAULT 0,
  player_seq INT NOT NULL DEFAULT 0,
  item_state CHAR(1) NOT NULL DEFAULT '1',
  item_code INT NOT NULL DEFAULT 0,
  item_equip CHAR(1) NOT NULL DEFAULT '0',
  item_amount INT NOT NULL DEFAULT 0,
  item_grade INT NOT NULL DEFAULT 0,
  item_level INT NOT NULL DEFAULT 0,
  item_class INT NOT NULL DEFAULT 0,
  item_limit INT NOT NULL DEFAULT 0,
  item_price INT NOT NULL DEFAULT 0,
  item_option0 INT NOT NULL DEFAULT 0,
  item_option1 INT NOT NULL DEFAULT 0,
  item_option2 INT NOT NULL DEFAULT 0,
  item_option3 INT NOT NULL DEFAULT 0,
  item_option4 INT NOT NULL DEFAULT 0,
  item_random INT NOT NULL DEFAULT 0,
  item_order INT NOT NULL DEFAULT 0,
  item_buydate VARCHAR(32) NOT NULL DEFAULT '',
  item_deletedate VARCHAR(32) NOT NULL DEFAULT ''
);

SET @missing_idx_tb_game_item_player = (
  SELECT COUNT(*) = 0
  FROM information_schema.statistics
  WHERE table_schema = DATABASE()
    AND table_name = 'tb_game_item'
    AND index_name = 'idx_tb_game_item_player'
);
SET @idx_tb_game_item_player_ddl = IF(
  @missing_idx_tb_game_item_player,
  'CREATE INDEX idx_tb_game_item_player ON tb_game_item (player_seq, item_state, item_equip)',
  'SELECT 1'
);
PREPARE idx_tb_game_item_player_stmt FROM @idx_tb_game_item_player_ddl;
EXECUTE idx_tb_game_item_player_stmt;
DEALLOCATE PREPARE idx_tb_game_item_player_stmt;

CREATE TABLE IF NOT EXISTS tb_game_skill (
  skill_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  member_seq INT NOT NULL DEFAULT 0,
  player_seq INT NOT NULL DEFAULT 0,
  skill_state CHAR(1) NOT NULL DEFAULT '1',
  skill_code INT NOT NULL DEFAULT 0,
  skill_equip INT NOT NULL DEFAULT 0,
  skill_level INT NOT NULL DEFAULT 0,
  skill_buydate VARCHAR(32) NOT NULL DEFAULT ''
);

CREATE TABLE IF NOT EXISTS tb_game_training (
  training_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  member_seq INT NOT NULL DEFAULT 0,
  player_seq INT NOT NULL DEFAULT 0,
  training_state CHAR(1) NOT NULL DEFAULT '1',
  training_code INT NOT NULL DEFAULT 0,
  training_type INT NOT NULL DEFAULT 0,
  training_level INT NOT NULL DEFAULT 0,
  training_kind VARCHAR(32) NOT NULL DEFAULT '',
  training_buydate VARCHAR(32) NOT NULL DEFAULT ''
);

CREATE TABLE IF NOT EXISTS tb_game_ceremony (
  ceremony_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  member_seq INT NOT NULL DEFAULT 0,
  player_seq INT NOT NULL DEFAULT 0,
  ceremony_state CHAR(1) NOT NULL DEFAULT '1',
  ceremony_code INT NOT NULL DEFAULT 0,
  ceremony_equip INT NOT NULL DEFAULT 0,
  ceremony_buydate VARCHAR(32) NOT NULL DEFAULT '',
  ceremony_deletedate VARCHAR(32) NOT NULL DEFAULT ''
);

CREATE TABLE IF NOT EXISTS tb_game_quest (
  quest_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  member_seq INT NOT NULL DEFAULT 0,
  player_seq INT NOT NULL DEFAULT 0,
  quest_code INT NOT NULL DEFAULT 0,
  quest_count INT NOT NULL DEFAULT 0,
  quest_playdate VARCHAR(32) NOT NULL DEFAULT ''
);

SET @missing_idx_tb_game_quest_player = (
  SELECT COUNT(*) = 0
  FROM information_schema.statistics
  WHERE table_schema = DATABASE()
    AND table_name = 'tb_game_quest'
    AND index_name = 'idx_tb_game_quest_player'
);
SET @idx_tb_game_quest_player_ddl = IF(
  @missing_idx_tb_game_quest_player,
  'CREATE INDEX idx_tb_game_quest_player ON tb_game_quest (player_seq, quest_code)',
  'SELECT 1'
);
PREPARE idx_tb_game_quest_player_stmt FROM @idx_tb_game_quest_player_ddl;
EXECUTE idx_tb_game_quest_player_stmt;
DEALLOCATE PREPARE idx_tb_game_quest_player_stmt;

SET @missing_idx_tb_game_quest_member = (
  SELECT COUNT(*) = 0
  FROM information_schema.statistics
  WHERE table_schema = DATABASE()
    AND table_name = 'tb_game_quest'
    AND index_name = 'idx_tb_game_quest_member'
);
SET @idx_tb_game_quest_member_ddl = IF(
  @missing_idx_tb_game_quest_member,
  'CREATE INDEX idx_tb_game_quest_member ON tb_game_quest (member_seq, quest_code)',
  'SELECT 1'
);
PREPARE idx_tb_game_quest_member_stmt FROM @idx_tb_game_quest_member_ddl;
EXECUTE idx_tb_game_quest_member_stmt;
DEALLOCATE PREPARE idx_tb_game_quest_member_stmt;

CREATE TABLE IF NOT EXISTS tb_game_record (
  record_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  player_seq INT NOT NULL UNIQUE,
  record_match INT NOT NULL DEFAULT 0,
  record_win INT NOT NULL DEFAULT 0,
  record_draw INT NOT NULL DEFAULT 0,
  record_mark INT NOT NULL DEFAULT 0,
  record_mvp INT NOT NULL DEFAULT 0,
  record_goal INT NOT NULL DEFAULT 0,
  record_allow INT NOT NULL DEFAULT 0,
  record_assist INT NOT NULL DEFAULT 0,
  record_cut INT NOT NULL DEFAULT 0,
  record_shoot INT NOT NULL DEFAULT 0,
  record_steal INT NOT NULL DEFAULT 0,
  record_tackle INT NOT NULL DEFAULT 0,
  record_pass INT NOT NULL DEFAULT 0,
  record_guard INT NOT NULL DEFAULT 0,
  record_tryshoot INT NOT NULL DEFAULT 0,
  record_trysteal INT NOT NULL DEFAULT 0,
  record_trytackle INT NOT NULL DEFAULT 0,
  record_trypass INT NOT NULL DEFAULT 0,
  record_tryguard INT NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS tb_game_record_26_02 LIKE tb_game_record;
CREATE TABLE IF NOT EXISTS tb_game_record_02_23 LIKE tb_game_record;
CREATE TABLE IF NOT EXISTS tb_game_record_26_06 LIKE tb_game_record;

CREATE TABLE IF NOT EXISTS tb_game_record_week LIKE tb_game_record;
SET @missing_record_date = (
  SELECT COUNT(*) = 0
  FROM information_schema.columns
  WHERE table_schema = DATABASE()
    AND table_name = 'tb_game_record_week'
    AND column_name = 'record_date'
);
SET @record_date_ddl = IF(
  @missing_record_date,
  'ALTER TABLE tb_game_record_week ADD COLUMN record_date DATE NOT NULL DEFAULT ''1970-01-01'' AFTER player_seq',
  'SELECT 1'
);
PREPARE record_date_stmt FROM @record_date_ddl;
EXECUTE record_date_stmt;
DEALLOCATE PREPARE record_date_stmt;

CREATE TABLE IF NOT EXISTS tb_game_ranking (
  ranking_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  player_seq INT NOT NULL DEFAULT 0,
  ranking_num INT NOT NULL DEFAULT 0,
  ranking_mark INT NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS tb_game_ranking_week (
  ranking_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  player_seq INT NOT NULL DEFAULT 0,
  ranking_date DATE NOT NULL DEFAULT (CURRENT_DATE),
  ranking_num INT NOT NULL DEFAULT 0,
  ranking_mark INT NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS tb_game_rank (
  player_seq INT NOT NULL PRIMARY KEY,
  rank_num INT NOT NULL DEFAULT 0,
  rank_code INT NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS tb_game_rank_26_02 LIKE tb_game_rank;
CREATE TABLE IF NOT EXISTS tb_game_rank_02_23 LIKE tb_game_rank;
CREATE TABLE IF NOT EXISTS tb_game_rank_26_06 LIKE tb_game_rank;

CREATE TABLE IF NOT EXISTS tb_game_gift (
  gift_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  player_seq INT NOT NULL DEFAULT 0,
  gifter_seq INT NOT NULL DEFAULT 0,
  item_seq INT NOT NULL DEFAULT 0,
  gift_msg VARCHAR(255) NOT NULL DEFAULT '',
  gift_createdate VARCHAR(32) NOT NULL DEFAULT ''
);

CREATE TABLE IF NOT EXISTS tb_game_card (
  card_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  player_seq INT NOT NULL DEFAULT 0,
  member_seq INT NOT NULL DEFAULT 0,
  card_code INT NOT NULL DEFAULT 0,
  card_equip1 INT NOT NULL DEFAULT 0,
  card_equip2 INT NOT NULL DEFAULT 0,
  card_equip3 INT NOT NULL DEFAULT 0,
  card_level INT NOT NULL DEFAULT 0,
  card_rank INT NOT NULL DEFAULT 0,
  card_position INT NOT NULL DEFAULT 0,
  card_area INT NOT NULL DEFAULT 0,
  card_skill INT NOT NULL DEFAULT 0,
  card_costume INT NOT NULL DEFAULT 0,
  card_tired INT NOT NULL DEFAULT 0,
  card_state CHAR(1) NOT NULL DEFAULT '1',
  card_deletedate VARCHAR(32) NOT NULL DEFAULT ''
);

CREATE TABLE IF NOT EXISTS tb_game_blacklist (
  player_seq INT NOT NULL DEFAULT 0,
  member_seq INT NOT NULL DEFAULT 0,
  target_seq INT NOT NULL DEFAULT 0,
  PRIMARY KEY (player_seq, target_seq)
);

CREATE TABLE IF NOT EXISTS tb_game_buddy (
  player_seq INT NOT NULL DEFAULT 0,
  member_seq INT NOT NULL DEFAULT 0,
  target_seq INT NOT NULL DEFAULT 0,
  PRIMARY KEY (player_seq, target_seq)
);

CREATE TABLE IF NOT EXISTS tb_game_hack (
  hack_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  member_seq INT NOT NULL DEFAULT 0,
  player_seq INT NOT NULL DEFAULT 0,
  hack_command INT NOT NULL DEFAULT 0,
  hack_type INT NOT NULL DEFAULT 0,
  hack_date VARCHAR(32) NOT NULL DEFAULT ''
);

USE xkick_log;

CREATE TABLE IF NOT EXISTS tb_log_connect_26_06 (
  connect_date VARCHAR(32) NOT NULL DEFAULT '',
  connect_server INT NOT NULL DEFAULT 0,
  connect_count INT NOT NULL DEFAULT 0,
  relay_count INT NOT NULL DEFAULT 0,
  room_count INT NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS tb_log_sale_26_06 (
  sale_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  member_seq INT NOT NULL DEFAULT 0,
  player_seq INT NOT NULL DEFAULT 0,
  object_seq INT NOT NULL DEFAULT 0,
  object_code INT NOT NULL DEFAULT 0,
  object_kind CHAR(1) NOT NULL DEFAULT '0',
  buy_kind CHAR(1) NOT NULL DEFAULT '0',
  sale_kind CHAR(1) NOT NULL DEFAULT '0',
  sale_price INT NOT NULL DEFAULT 0,
  sale_amount INT NOT NULL DEFAULT 0,
  store_kind CHAR(1) NOT NULL DEFAULT '0',
  sale_buydate VARCHAR(32) NOT NULL DEFAULT ''
);

CREATE TABLE IF NOT EXISTS tb_log_sale_26_02 LIKE tb_log_sale_26_06;
CREATE TABLE IF NOT EXISTS tb_log_sale_02_23 LIKE tb_log_sale_26_06;

CREATE TABLE IF NOT EXISTS tb_mst_playlog_26_06 (
  playlog_seq INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  member_seq INT NOT NULL DEFAULT 0,
  player_seq INT NOT NULL DEFAULT 0,
  server_code INT NOT NULL DEFAULT 0,
  playlog_exp_in INT NOT NULL DEFAULT 0,
  playlog_exp_out INT NOT NULL DEFAULT 0,
  playlog_match_in INT NOT NULL DEFAULT 0,
  playlog_match_out INT NOT NULL DEFAULT 0,
  playlog_cash_in INT NOT NULL DEFAULT 0,
  playlog_cash_out INT NOT NULL DEFAULT 0,
  playlog_point_in INT NOT NULL DEFAULT 0,
  playlog_point_out INT NOT NULL DEFAULT 0,
  playlog_remoteip VARCHAR(64) NOT NULL DEFAULT '',
  playlog_date_in VARCHAR(32) NOT NULL DEFAULT '',
  playlog_date_out VARCHAR(32) NOT NULL DEFAULT ''
);
