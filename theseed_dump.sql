-- --------------------------------------------------------
-- Hôte :                        localhost
-- Version du serveur:           5.7.24 - MySQL Community Server (GPL)
-- SE du serveur:                Win64
-- HeidiSQL Version:             10.2.0.5599
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


-- Listage de la structure de la base pour theseed
CREATE DATABASE IF NOT EXISTS `theseed` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `theseed`;

-- Listage de la structure de la table theseed. accounts
CREATE TABLE IF NOT EXISTS `accounts` (
  `account_id` int(11) NOT NULL,
  `name` varchar(50) NOT NULL DEFAULT '0',
  `password` varchar(255) NOT NULL DEFAULT '',
  `email` varchar(50) NOT NULL DEFAULT '',
  PRIMARY KEY (`account_id`),
  UNIQUE KEY `id` (`account_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table theseed. broker
CREATE TABLE IF NOT EXISTS `broker` (
  `broker_id` double DEFAULT NULL,
  `item_id` double DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table theseed. inventory
CREATE TABLE IF NOT EXISTS `inventory` (
  `inventory_id` double NOT NULL DEFAULT '0',
  `player_id` int(11) NOT NULL DEFAULT '0',
  `item_id` double NOT NULL DEFAULT '0',
  PRIMARY KEY (`inventory_id`),
  UNIQUE KEY `id` (`inventory_id`),
  KEY `player_id` (`player_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table theseed. items
CREATE TABLE IF NOT EXISTS `items` (
  `item_id` double DEFAULT NULL,
  `name` varchar(50) DEFAULT NULL,
  `statistics` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table theseed. skills
CREATE TABLE IF NOT EXISTS `skills` (
  `skill_id` double DEFAULT NULL,
  `player_id` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table theseed. users
CREATE TABLE IF NOT EXISTS `users` (
  `player_id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL DEFAULT '0',
  `class` int(11) DEFAULT NULL,
  `currency` float DEFAULT NULL,
  `mp` int(11) DEFAULT NULL,
  `re` int(11) DEFAULT NULL,
  `hp` double DEFAULT NULL,
  `x` double DEFAULT NULL,
  `y` double DEFAULT NULL,
  `z` double DEFAULT NULL,
  PRIMARY KEY (`player_id`),
  UNIQUE KEY `id` (`player_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

-- Les données exportées n'étaient pas sélectionnées.

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;