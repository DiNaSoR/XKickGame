CREATE DATABASE IF NOT EXISTS xkick CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
CREATE DATABASE IF NOT EXISTS xkick_log CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
CREATE DATABASE IF NOT EXISTS xkick_sample CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;

GRANT USAGE ON *.* TO 'xkick'@'%' IDENTIFIED BY 'xkick_dev_password';
GRANT ALL PRIVILEGES ON xkick.* TO 'xkick'@'%';
GRANT ALL PRIVILEGES ON xkick_log.* TO 'xkick'@'%';
GRANT ALL PRIVILEGES ON xkick_sample.* TO 'xkick'@'%';
FLUSH PRIVILEGES;
