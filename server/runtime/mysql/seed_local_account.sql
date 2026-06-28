USE xkick;

ALTER TABLE tb_mst_member
  MODIFY member_partner VARCHAR(64) NOT NULL DEFAULT 'enterplay';

INSERT INTO tb_mst_member (
  member_id, member_pass, member_name, member_register, member_state, member_partner,
  member_ip, member_speed, member_createdate, member_logindate, member_deletedate,
  member_blockdate, member_releasedate
) VALUES (
  'test', '1234', 'Tester', '1111111000001', 1, 'enterplay',
  '127.0.0.1', 0, NOW(), '0000-00-00 00:00:00', '0000-00-00 00:00:00',
  '0000-00-00 00:00:00', '0000-00-00 00:00:00'
) ON DUPLICATE KEY UPDATE
  member_pass = VALUES(member_pass),
  member_state = 1,
  member_name = VALUES(member_name);

SET @local_member_seq = (SELECT member_seq FROM tb_mst_member WHERE member_id = 'test' LIMIT 1);

INSERT INTO tb_mst_meminfo (member_register, meminfo_memo)
SELECT '1111111000001', 'Local test account'
WHERE NOT EXISTS (SELECT 1 FROM tb_mst_meminfo WHERE member_register = '1111111000001');

INSERT INTO tb_game_trio (
  member_seq, trio_cash, trio_point, trio_credit, trio_allpoint, trio_allcredit,
  trio_lastseq, trio_server, trio_count, trio_tutorial, trio_quest, trio_camera,
  trio_camerateam, trio_radian, trio_zoom, trio_target, trio_shadow, trio_label,
  trio_sound, trio_music, trio_invite, trio_whisper, trio_friend, trio_up, trio_down,
  trio_left, trio_right, trio_leftshoot, trio_rightshoot, trio_longpass, trio_shortpass,
  trio_screen, trio_tackle, trio_steal, trio_quick, trio_skill1, trio_skill2,
  trio_skill3, trio_skill4, trio_skillcode1, trio_skillcode2, trio_skillcode3,
  trio_skillcode4, trio_logindate, trio_deletedate
) VALUES (
  @local_member_seq, 10000000, 10000000, 1000, 0, 0,
  0, 0, 3, 63, 7, 12,
  1, 0, 2, 10, 0, 0,
  8, 16, 1, 1, 1, 222, 230,
  225, 227, 32, 33, 30, 31,
  17, 16, 18, 42, 46, 45,
  47, 57, 0, 0, 0,
  0, NOW(), '0000-00-00 00:00:00'
) ON DUPLICATE KEY UPDATE
  trio_cash = VALUES(trio_cash),
  trio_point = VALUES(trio_point),
  trio_credit = VALUES(trio_credit),
  trio_tutorial = VALUES(trio_tutorial),
  trio_quest = VALUES(trio_quest);

INSERT INTO tb_game_player (
  member_seq, player_order, player_name, player_gender, player_skin, player_face,
  player_hair, player_shirts, player_pants, player_shoes, player_createdate,
  player_level, player_faculty, player_position, player_skill
)
SELECT @local_member_seq, 0, '[FW]test', 1, 11, 101101111,
  102101101, 201001101, 202001101, 204001101, NOW(),
  10, 21, 10, 100
WHERE NOT EXISTS (
  SELECT 1 FROM tb_game_player WHERE member_seq = @local_member_seq AND player_order = 0
);

INSERT INTO tb_game_player (
  member_seq, player_order, player_name, player_gender, player_skin, player_face,
  player_hair, player_shirts, player_pants, player_shoes, player_createdate,
  player_level, player_faculty, player_position, player_skill
)
SELECT @local_member_seq, 1, '[MF]test', 1, 11, 101101111,
  102101101, 201001101, 202001101, 204001101, NOW(),
  10, 21, 20, 100
WHERE NOT EXISTS (
  SELECT 1 FROM tb_game_player WHERE member_seq = @local_member_seq AND player_order = 1
);

INSERT INTO tb_game_player (
  member_seq, player_order, player_name, player_gender, player_skin, player_face,
  player_hair, player_shirts, player_pants, player_shoes, player_createdate,
  player_level, player_faculty, player_position, player_skill
)
SELECT @local_member_seq, 2, '[GK]test', 1, 11, 101101111,
  102101101, 201001101, 202001101, 204001101, NOW(),
  10, 21, 40, 100
WHERE NOT EXISTS (
  SELECT 1 FROM tb_game_player WHERE member_seq = @local_member_seq AND player_order = 2
);

INSERT IGNORE INTO tb_game_faculty (player_seq)
SELECT player_seq FROM tb_game_player WHERE member_seq = @local_member_seq;

INSERT IGNORE INTO tb_game_setting (player_seq)
SELECT player_seq FROM tb_game_player WHERE member_seq = @local_member_seq;

UPDATE tb_game_player
SET player_level = 50,
    player_exp = 1559074,
    player_rand = 0,
    player_shopstate = 0
WHERE member_seq = @local_member_seq
  AND (
    player_level <> 50
    OR player_exp <> 1559074
    OR player_rand <> 0
    OR player_shopstate <> 0
  );

INSERT IGNORE INTO tb_game_record (player_seq)
SELECT player_seq FROM tb_game_player WHERE member_seq = @local_member_seq;

INSERT IGNORE INTO tb_game_record_26_02
SELECT * FROM tb_game_record WHERE player_seq IN (
  SELECT player_seq FROM tb_game_player WHERE member_seq = @local_member_seq
);

INSERT IGNORE INTO tb_game_record_02_23
SELECT * FROM tb_game_record WHERE player_seq IN (
  SELECT player_seq FROM tb_game_player WHERE member_seq = @local_member_seq
);

INSERT IGNORE INTO tb_game_record_26_06
SELECT * FROM tb_game_record WHERE player_seq IN (
  SELECT player_seq FROM tb_game_player WHERE member_seq = @local_member_seq
);

INSERT IGNORE INTO tb_game_rank (player_seq, rank_num, rank_code)
SELECT player_seq, 0, 0 FROM tb_game_player WHERE member_seq = @local_member_seq;

INSERT IGNORE INTO tb_game_rank_26_02
SELECT * FROM tb_game_rank WHERE player_seq IN (
  SELECT player_seq FROM tb_game_player WHERE member_seq = @local_member_seq
);

INSERT IGNORE INTO tb_game_rank_02_23
SELECT * FROM tb_game_rank WHERE player_seq IN (
  SELECT player_seq FROM tb_game_player WHERE member_seq = @local_member_seq
);

INSERT IGNORE INTO tb_game_rank_26_06
SELECT * FROM tb_game_rank WHERE player_seq IN (
  SELECT player_seq FROM tb_game_player WHERE member_seq = @local_member_seq
);

INSERT IGNORE INTO tb_game_buddy (player_seq, member_seq, target_seq)
SELECT player.player_seq, player.member_seq, target.player_seq
FROM tb_game_player AS player
JOIN tb_game_player AS target
  ON target.member_seq = player.member_seq
 AND target.player_seq <> player.player_seq
WHERE player.member_seq = @local_member_seq;
