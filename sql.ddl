-- phpMyAdmin SQL Dump
-- version 4.0.10deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Oct 07, 2015 at 12:41 PM
-- Server version: 5.5.44-0ubuntu0.14.04.1
-- PHP Version: 5.5.9-1ubuntu4.13

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

--
-- Database: `snappy`
--
CREATE DATABASE IF NOT EXISTS `snappy` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `snappy`;

DELIMITER $$
--
-- Procedures
--
DROP PROCEDURE IF EXISTS `SetInstanceID`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `SetInstanceID`(IN id CHAR(36))
BEGIN
	DECLARE setID CHAR(36) DEFAULT NULL;
	DECLARE stmt VARCHAR(1024);

	SELECT COLUMN_DEFAULT
	INTO setID
	FROM INFORMATION_SCHEMA.COLUMNS
		WHERE TABLE_SCHEMA = 'snappy'
		AND TABLE_NAME = 'ScoreChangeCount'
		AND COLUMN_NAME = 'InstanceID';

	IF setID IS NULL THEN
		SET @SQL := CONCAT('ALTER TABLE ScoreChangeCount MODIFY InstanceID CHAR(36) DEFAULT ''', id, '''');
		PREPARE stmt FROM @SQL;
		EXECUTE stmt;
		DEALLOCATE PREPARE stmt;
	END IF;

	SELECT setID;
END$$

DROP PROCEDURE IF EXISTS `UnSetInstanceID`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `UnSetInstanceID`()
BEGIN
	ALTER TABLE ScoreChangeCount MODIFY InstanceID char(36) NOT NULL;
END$$

DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `InstanceSettings`
--

DROP TABLE IF EXISTS `InstanceSettings`;
CREATE TABLE IF NOT EXISTS `InstanceSettings` (
  `InstanceID` char(36) NOT NULL,
  `Hostname` varchar(1024) NOT NULL,
  `StartTime` datetime NOT NULL,
  `EndTIme` datetime NOT NULL,
  `PreviousInstanceID` char(36) DEFAULT NULL,
  `Discretizer` double NOT NULL,
  `DiscountFactor` double NOT NULL,
  `ExplorationRate` double NOT NULL,
  `FlapRate` double NOT NULL,
  `LearningRate` int(11) NOT NULL,
  `ScaledExplorer` tinyint(1) NOT NULL,
  `Games` int(11) NOT NULL,
  `Frames` int(11) NOT NULL,
  `BestGame` int(11) NOT NULL,
  `HighScore` int(11) NOT NULL,
  `ScoreChangeCount` int(11) NOT NULL,
  `TotalGames` int(11) NOT NULL,
  `TotalFrames` int(11) NOT NULL,
  `TotalBestGame` int(11) NOT NULL,
  `TotalHighScore` int(11) NOT NULL,
  `TotalScoreChangeCount` int(11) NOT NULL,
  PRIMARY KEY (`InstanceID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Instance_StateVariables`
--

DROP TABLE IF EXISTS `Instance_StateVariables`;
CREATE TABLE IF NOT EXISTS `Instance_StateVariables` (
  `InstanceID` char(36) NOT NULL,
  `StateVariableID` int(11) NOT NULL,
  PRIMARY KEY (`InstanceID`,`StateVariableID`),
  KEY `StateVariableID` (`StateVariableID`),
  KEY `StateVariableID_2` (`StateVariableID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `ScoreChangeCount`
--

DROP TABLE IF EXISTS `ScoreChangeCount`;
CREATE TABLE IF NOT EXISTS `ScoreChangeCount` (
  `InstanceID` char(36) NOT NULL,
  `GameNumber` int(11) NOT NULL,
  `Score` int(11) NOT NULL,
  PRIMARY KEY (`InstanceID`,`GameNumber`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `StateVariables`
--

DROP TABLE IF EXISTS `StateVariables`;
CREATE TABLE IF NOT EXISTS `StateVariables` (
  `ID` int(11) NOT NULL,
  `Name` varchar(1024) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `Instance_StateVariables`
--
ALTER TABLE `Instance_StateVariables`
  ADD CONSTRAINT `FK_StateVariableID` FOREIGN KEY (`StateVariableID`) REFERENCES `StateVariables` (`ID`);

