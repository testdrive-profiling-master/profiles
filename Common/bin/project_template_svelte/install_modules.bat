@echo off
if not exist node_modules (
	echo "*I: Installing dependency modules first..."
	npm install
)
