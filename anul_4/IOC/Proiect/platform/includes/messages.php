<?php

    class JSON_CONSTS
    {
        const MESSAGE_TYPE = "type";
        const MESSAGE_DESC = "text";
        
        const PROFILE_FIRSTNAME = "first_name";
        const PROFILE_LASTNAME = "last_name";
        const PROFILE_IMAGE = "img_url";
        const PROFILE_BIRTHDAY = "birthday";
        const PROFILE_DESCRIPTION = "description";
        
        const ACTIVITY_TAG = "activity";
        
        const ACTIVITY_LAT = "lat";
        const ACTIVITY_LONG = "long";
        const ACTIVITY_TIME = "time";
        const ACTIVITY_AUTHOR = "author";
        const ACTIVITY_TAGS_LIST = "tags";
        const ACTIVITY_MAX_MEMBERS = "max_members";
        const ACTIVITY_CRT_MEMBERS = "crt_members";
    }

    class MESSAGE_TYPE 
    {
        const DB_NOT_REACHABLE = 0;
        
        const LOGIN_SUCCESS = 1;
        const LOGIN_FAIL = 2;
        const LOGIN_NO_DATA = 3;
        const LOGIN_INVALID_ACTION = 4;
        const LOGIN_REQUIRED = 5;
        const LOGIN_SUCCESS_LOGOUT = 6;
        const LOGIN_ALREADY_LOGGED_IN = 7;
        
        const REGISTER_NO_DATA = 8;
        const REGISTER_USERNAME_ALREADY_TAKEN = 9;
        const REGISTER_SUCCESS_AND_LOGGED_IN = 10;
        
        const PROFILE_INVALID_ACTION = 11;
        const PROFILE_NOT_FOUND = 12;
        const PROFILE_VIEW_INFO = 13;
        const PROFILE_NO_DATA = 18;
        const PROFILE_UPDATED_SUCCESSFULLY = 19;
        const PROFILE_INVALID_IMAGE = 24;
        
        const ACTIVITY_INVALID_ACTION = 14;
        const ACTIVITY_LIST = 15;
        const ACTIVITY_NO_DATA = 16;
        const ACTIVITY_SUCCESSFULLY_ADDED = 17;
        const ACTIVITY_INVALID_ATTEND = 20;
        const ACTIVITY_SUCCESSFULLY_ATTENDED = 21;
        const ACTIVITY_INVALID_QUIT = 22;
        const ACTIVITY_SUCCESSFULLY_QUITTED = 23;
        const ACTIVITY_ALREADY_ATTENDING = 24;
    };
    
    class MESSAGE_DESC
    {
        const DB_NOT_REACHABLE = "DB server is down";
        
        const LOGIN_SUCCESS = "Login successful";
        const LOGIN_FAIL = "Username & password combination not found";
        const LOGIN_NO_DATA = "No authentication data was received";
        const LOGIN_INVALID_ACTION = "Invalid authentication action - check parameter ?action=";
        const LOGIN_REQUIRED = "You need to be logged in to perform that action";
        const LOGIN_SUCCESS_LOGOUT = "You've been successfully logged out";
        const LOGIN_ALREADY_LOGGED_IN = "You're already logged in";
        
        const REGISTER_NO_DATA = "Not registration data was received";
        const REGISTER_USERNAME_ALREADY_TAKEN = "This username is already taken";
        const REGISTER_SUCCESS_AND_LOGGED_IN = "Registration successful; also logged in";
        
        const PROFILE_INVALID_ACTION = "Invalid profile action - check parameter ?action=";
        const PROFILE_NOT_FOUND = "A profile for the given username was not found";
        const PROFILE_VIEW_INFO = "Profile information";
        const PROFILE_NO_DATA = "No profile data was received";
        const PROFILE_UPDATED_SUCCESSFULLY = "Profile data has been updated";
        const PROFILE_INVALID_IMAGE = "The image is not valid (check size, type, etc.)";
        
        const ACTIVITY_INVALID_ACTION = "Invalid activity action - check parameter ?action=";
        const ACTIVITY_LIST = "List of activities, given the tags";
        const ACTIVITY_NO_DATA = "Not enough data was provided";
        const ACTIVITY_SUCCESSFULLY_ADDED = "Successfully added a new activity";
        const ACTIVITY_INVALID_ATTEND = "Can't attend this activity (no slots available or too far)";
        const ACTIVITY_SUCCESSFULLY_ATTENDED = "Successfully attended this activity";
        const ACTIVITY_INVALID_QUIT = "Can't quit an activity you're not attending";
        const ACTIVITY_SUCCESSFULLY_QUITTED = "Successfully quitted this activity";
        const ACTIVITY_ALREADY_ATTENDING = "You're already attending this activity";
    };

?>