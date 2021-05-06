-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.7.24 - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL Version:             10.2.0.5599
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


-- Dumping database structure for theseed
DROP DATABASE IF EXISTS `theseed`;
CREATE DATABASE IF NOT EXISTS `theseed` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `theseed`;

-- Dumping structure for table theseed.accounts
DROP TABLE IF EXISTS `accounts`;
CREATE TABLE IF NOT EXISTS `accounts` (
  `account_id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL DEFAULT '0',
  `password` varchar(255) NOT NULL DEFAULT '',
  `email` varchar(50) NOT NULL DEFAULT '',
  `VIP` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`account_id`),
  UNIQUE KEY `id` (`account_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

-- Dumping data for table theseed.accounts: ~2 rows (approximately)
/*!40000 ALTER TABLE `accounts` DISABLE KEYS */;
REPLACE INTO `accounts` (`account_id`, `name`, `password`, `email`, `VIP`) VALUES
	(1, 'test', 'puissant', '', 0),
	(2, 'testo', 'test', '', 0);
/*!40000 ALTER TABLE `accounts` ENABLE KEYS */;

-- Dumping structure for table theseed.appearance
DROP TABLE IF EXISTS `appearance`;
CREATE TABLE IF NOT EXISTS `appearance` (
  `character_id` int(11) NOT NULL,
  `head` int(11) NOT NULL,
  `body` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Dumping data for table theseed.appearance: ~0 rows (approximately)
/*!40000 ALTER TABLE `appearance` DISABLE KEYS */;
/*!40000 ALTER TABLE `appearance` ENABLE KEYS */;

-- Dumping structure for table theseed.broker
DROP TABLE IF EXISTS `broker`;
CREATE TABLE IF NOT EXISTS `broker` (
  `broker_id` double DEFAULT NULL,
  `item_id` double DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table theseed.broker: ~0 rows (approximately)
/*!40000 ALTER TABLE `broker` DISABLE KEYS */;
/*!40000 ALTER TABLE `broker` ENABLE KEYS */;

-- Dumping structure for table theseed.currentplayerstats
DROP TABLE IF EXISTS `currentplayerstats`;
CREATE TABLE IF NOT EXISTS `currentplayerstats` (
  `user_id` int(20) NOT NULL,
  `hp` double NOT NULL,
  `mp` double NOT NULL,
  `attack` double NOT NULL,
  `critRate` double NOT NULL,
  `critP` double NOT NULL,
  `defense` double NOT NULL,
  `re` int(11) NOT NULL,
  PRIMARY KEY (`user_id`),
  UNIQUE KEY `user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Dumping data for table theseed.currentplayerstats: ~2 rows (approximately)
/*!40000 ALTER TABLE `currentplayerstats` DISABLE KEYS */;
REPLACE INTO `currentplayerstats` (`user_id`, `hp`, `mp`, `attack`, `critRate`, `critP`, `defense`, `re`) VALUES
	(2, 100, 50, 10, 5, 0.2, 10, 1000),
	(3, 100, 50, 10, 5, 0.2, 10, 1000);
/*!40000 ALTER TABLE `currentplayerstats` ENABLE KEYS */;

-- Dumping structure for table theseed.inventory
DROP TABLE IF EXISTS `inventory`;
CREATE TABLE IF NOT EXISTS `inventory` (
  `inventory_id` double NOT NULL DEFAULT '0',
  `player_id` int(11) NOT NULL DEFAULT '0',
  `item_id` double NOT NULL DEFAULT '0',
  PRIMARY KEY (`inventory_id`),
  UNIQUE KEY `id` (`inventory_id`),
  KEY `player_id` (`player_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table theseed.inventory: ~0 rows (approximately)
/*!40000 ALTER TABLE `inventory` DISABLE KEYS */;
/*!40000 ALTER TABLE `inventory` ENABLE KEYS */;

-- Dumping structure for table theseed.items
DROP TABLE IF EXISTS `items`;
CREATE TABLE IF NOT EXISTS `items` (
  `item_id` double DEFAULT NULL,
  `name` varchar(50) DEFAULT NULL,
  `statistics` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table theseed.items: ~0 rows (approximately)
/*!40000 ALTER TABLE `items` DISABLE KEYS */;
/*!40000 ALTER TABLE `items` ENABLE KEYS */;

-- Dumping structure for table theseed.skillslearned
DROP TABLE IF EXISTS `skillslearned`;
CREATE TABLE IF NOT EXISTS `skillslearned` (
  `skill_id` double DEFAULT NULL,
  `player_id` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table theseed.skillslearned: ~0 rows (approximately)
/*!40000 ALTER TABLE `skillslearned` DISABLE KEYS */;
/*!40000 ALTER TABLE `skillslearned` ENABLE KEYS */;

-- Dumping structure for table theseed.users
DROP TABLE IF EXISTS `users`;
CREATE TABLE IF NOT EXISTS `users` (
  `account_id` bigint(11) NOT NULL,
  `player_id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL DEFAULT '0',
  `class` int(11) DEFAULT '0',
  `currency` float DEFAULT '0',
  `x` double DEFAULT '82',
  `y` double DEFAULT '0.1',
  `z` double DEFAULT '779',
  `rotationx` double NOT NULL DEFAULT '0',
  `rotationy` double NOT NULL DEFAULT '0',
  `rotationz` double NOT NULL DEFAULT '0',
  `region` int(11) NOT NULL DEFAULT '0',
  `level` int(11) NOT NULL DEFAULT '1',
  `exp` double NOT NULL DEFAULT '0',
  `lastEnterWorld` varchar(100) DEFAULT NULL,
  `lastLeaveWorld` varchar(100) DEFAULT NULL,
  `isAlive` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`player_id`),
  UNIQUE KEY `id` (`player_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

-- Dumping data for table theseed.users: ~2 rows (approximately)
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
REPLACE INTO `users` (`account_id`, `player_id`, `name`, `class`, `currency`, `x`, `y`, `z`, `rotationx`, `rotationy`, `rotationz`, `region`, `level`, `exp`, `lastEnterWorld`, `lastLeaveWorld`, `isAlive`) VALUES
	(1, 2, 'test', 0, 0, 71.35069, 0.05, 798.4075, 0, 0, 0, 0, 1, 0, NULL, NULL, 1),
	(2, 3, 'testo', 0, 0, 82, 0.1, 779, 0, 0, 0, 0, 1, 0, NULL, NULL, 1);
/*!40000 ALTER TABLE `users` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
