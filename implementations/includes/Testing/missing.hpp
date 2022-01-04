#pragma once

namespace Testing {

/*
Classes missing constructors */

class Default {
public:
	Default();
	Default(const Default&);
	Default(Default&&);
	Default& operator=(const Default&);
	Default& operator=(Default&&);
	~Default();
private:
};

class Nothing {
private:
	Nothing();
	Nothing(const Nothing&);
	Nothing(Nothing&&);
	Nothing& operator=(const Nothing&);
	Nothing& operator=(Nothing&&);
public:
	~Nothing();
};

class DefaultOnly {
private:
	DefaultOnly(const DefaultOnly&);
	DefaultOnly(DefaultOnly&&);
	DefaultOnly& operator=(const DefaultOnly&);
	DefaultOnly& operator=(DefaultOnly&&);
public:
	DefaultOnly();
	~DefaultOnly();
};

class DefaultCopyOnly {
private:
	DefaultCopyOnly(DefaultCopyOnly&&);
	DefaultCopyOnly& operator=(const DefaultCopyOnly&);
	DefaultCopyOnly& operator=(DefaultCopyOnly&&);
public:
	DefaultCopyOnly();
	DefaultCopyOnly(const DefaultCopyOnly&);
	~DefaultCopyOnly();
};

class DCM_ONLY {
private:
	DCM_ONLY& operator=(const DCM_ONLY&);
	DCM_ONLY& operator=(DCM_ONLY&&);
public:
	DCM_ONLY();
	DCM_ONLY(DCM_ONLY&&);
	DCM_ONLY(const DCM_ONLY&);
	~DCM_ONLY();
};

}
