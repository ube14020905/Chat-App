
--create users table for authentication
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    online BOOLEAN DEFAULT 0
);
--maintain message log in the db for msg history
CREATE TABLE messages (
    id INT AUTO_INCREMENT PRIMARY KEY,
    sender VARCHAR(50),
    receiver VARCHAR(50),
    message TEXT,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    delivered BOOLEAN DEFAULT 0
);


--event to delete messages daily
CREATE EVENT delete_old_messages
ON SCHEDULE EVERY 1 DAY
DO
  DELETE FROM messages
  WHERE timestamp < NOW() - INTERVAL 1 DAY;
