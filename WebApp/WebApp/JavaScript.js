﻿if (!('ucfirst' in String.prototype)) {
    String.prototype.ucfirst = function() {
        return this.charAt(0).toUpperCase() + this.substr(1);
    }
}
(function($) {
    $.fn.defchat = function(options) {
        var settings = {
            'domain': 'localhost',
            'username': null,
            'password': null,
            'my_username': '',
            'textinput': $('.chat-box input.text, #chat_input'),
            'form': $('form.chat-form'),
            'chatlist': $('.chat-list'),
            'chat_host_fallbacks': [],
            'userslist': $('.users-list'),
            'buttons': $('.chat-box ul.buttons'),
            'submitbutton': $('.send_message_button'),
            'usercount': $('.usercount'),
            'handler': null,
            'force_handler': null,
            'subject': '',
            'room': 'mymucroom@conference.localhost',
            'private_room': 'mymucroom@conference.localhost',
            'throttlechat': false,
            'throttle_message': null,
            'add_moderator_url': null,
            'remove_moderator_url': null,
            'media_url': '',
            'hashtag_url': '',
            'ban_url': null,
            'silence_url': null,
            'is_broadcasting': false,
            'is_moderator': false,
            'is_age_verified': '',
            'is_private_show_owner': false,
            'allow_tipping': true,
            'domroot': this,
            'current_subject': '',
            'default_subject': '',
            'login_required_callback': null,
            'submenu_profile_url': null,
            'emoticon_overlay_url': null,
            'tipping_current_tokens_url': null,
            'welcome_message': null,
            'app_info_json': null,
            'welcome_warning': null,
            'first_post_warning': null,
            'parse_subject': null,
            'interested_in': [],
            'group_show_tokens_per_minute_url': null,
            'group_show_request_url': null,
            'group_show_decline_url': null,
            'group_show_cancel_url': null,
            'return_from_away_url': null,
            'spy_on_private_show_tokens_per_minute_url': null,
            'spy_on_private_show_request_url': null,
            'private_show_tokens_per_minute_url': null,
            'is_private_show_owner_url': null,
            'private_show_request_url': null,
            'private_show_approve_url': null,
            'private_show_decline_url': null,
            'private_show_cancel_url': null,
            'get_chat_user_menu_url': null,
            'get_user_panel_data_url': null,
            'spy_on_cams_url': null,
            'get_panel_url': null,
            'cancel_tip_goal_url': null,
            'tips_in_last_24_url': null,
            'scroll_on_load': true,
            'room_goal': {},
            'debug_mode': false,
            'num_users_waiting_for_group_show': 0,
            'num_users_required_for_group_show': 0,
            'html_chat_poll_url': '',
            'saved_scroll_top': 0,
            'scroll_saving_allowed': true,
            'site_name': '',
            'support_email': '',
            'silence_broadcasters': 'false',
            'users_that_can_chat': '',
            'show_emoticons': true,
            'emoticon_autocomplete_delay': '0',
            'chat_settings_url': '',
            'font_size': '9pt',
            'sort_users_key': 'a',
            'room_entry_for': 'orgb',
            'room_leave_for': 'orgb',
            'chat_ignore_list_url': '',
            'chat_mod_list_url': '',
            'chat_fan_list_url': '',
            'chat_ban_list_url': '',
            'chat_geo_list_url': '',
            'load_ignored_users': '',
            'min_tips_reqd_to_vote': 2147483647,
            'initial_viewer_count': null,
            'change_room_title_url': '',
            'ignored_emoticons': [],
            'v_tip_vol': 10,
            'b_tip_vol': 10,
            'broadcaster_gender': '',
            'private_price': '',
            'group_price': '',
            'spy_price': '',
            'mod_status_changed': false,
            'player_background_image_url': '',
            'do_not_connect': null,
            'password_room_login_url': null,
            'broadcaster_on_new_chat': null,
        };
        window.defchat_settings = settings;
        if (options) {
            $.extend(settings, options);
        }
        $.extend(settings, {
            'user': settings.username + '@' + settings.domain,
            'muc_nickname': settings.username,
            'chat_host': 'http://' + settings.domain + ':5280/http-bind'
        });
        if (options) {
            $.extend(settings, options);
        }
        if (settings.force_handler != null) {
            settings.handler = settings.force_handler;
        } else {
            settings.handler = ws_handler;
        }
        settings.performer = settings.room;
        if (settings.performer.indexOf('@') !== -1) {
            settings.performer = settings.room.split('@')[0];
        }
        var session_metrics = {
            'messageCount': 0,
            'messageSuccessCount': 0,
            'tipSuccessCount': 0,
            'tipSuccessTotal': 0,
            'tipFailCount': 0,
            'tipFailTotal': 0,
            'loadTimestamp': 0,
            'navigationType': undefined,
            'refreshMeta': {
                'cbVersion': 1,
                'href': '',
                'unloadTimestamp': 0,
                'count': 0,
            },
            'sent': false,
        };

        function careAboutMetrics() {
            return settings.username !== null && settings.username !== '' && settings.username.indexOf('__anonymous__') !== 0
        }

        function onLoad() {
            if (window.sessionStorage !== undefined && careAboutMetrics()) {
                session_metrics.loadTimestamp = new Date().getTime();
                try {
                    var payload = window.sessionStorage.getItem("refreshMeta");
                    if (payload !== null) {
                        session_metrics.refreshMeta = Object.assign({}, session_metrics.refreshMeta, JSON.parse(payload));
                    }
                } catch (e) {}
                session_metrics.navigationType = getNavigationType()
            }
        }
        onLoad();

        function getNavigationType() {
            if (window.performance !== undefined) {
                var navigationTimingEntries = (window.performance.getEntriesByType !== undefined ? window.performance.getEntriesByType("navigation") : []);
                if (navigationTimingEntries.length > 0) {
                    switch (navigationTimingEntries[0].type) {
                        case "navigate":
                            return 0;
                        case "reload":
                            return 1;
                        case "back_forward":
                            return 2;
                        case "prerender":
                            return 3;
                        default:
                            return 255;
                    }
                } else {
                    return window.performance.navigation.type;
                }
            } else {
                return undefined;
            }
        }

        function wasRefresh() {
            if (session_metrics.navigationType === 1) {
                return session_metrics.refreshMeta["cbVersion"] === 1;
            } else if (session_metrics.navigationType === 0 || this.navigationType === undefined) {
                return (session_metrics.refreshMeta["href"] === window.location.href && session_metrics.refreshMeta["unloadTimestamp"] > session_metrics.loadTimestamp - 10000 && session_metrics.refreshMeta["cbVersion"] === 1)
            } else {
                return false;
            }
        }

        function saveRefreshMeta() {
            if (window.sessionStorage !== undefined) {
                if (wasRefresh()) {
                    session_metrics.refreshMeta["count"] = 1;
                } else {
                    session_metrics.refreshMeta["count"] = 0;
                }
                session_metrics.refreshMeta = {
                    "cbVersion": 1,
                    "href": window.location.href,
                    "unloadTimestamp": new Date().getTime(),
                    "count": session_metrics.refreshMeta["count"],
                };
                try {
                    window.sessionStorage.setItem("refreshMeta", JSON.stringify(session_metrics.refreshMeta));
                } catch (e) {}
            }
        }

        function sendMetrics() {
            if (!session_metrics.sent && window.newrelic && careAboutMetrics()) {
                saveRefreshMeta();
                window.newrelic.addPageAction('SessionMetrics', {
                    "attributes.messageSuccessCount": session_metrics.messageSuccessCount,
                    "attributes.messageFailCount": (session_metrics.messageCount - session_metrics.messageSuccessCount),
                    "attributes.tipSuccessCount": session_metrics.tipSuccessCount,
                    "attributes.tipSuccessTotal": session_metrics.tipSuccessTotal,
                    "attributes.tipFailCount": session_metrics.tipFailCount,
                    "attributes.tipFailTotal": session_metrics.tipFailTotal,
                    "attributes.refreshCount": (window.sessionStorage !== undefined ? session_metrics.refreshMeta.count : undefined),
                })
                session_metrics.sent = true;
            }
        }
        $(window).bind('beforeunload', sendMetrics);
        $(window).bind('unload', sendMetrics);

        function consolelog(msg) {}

        function update_tips_in_last_24() {
            $.ajax({
                url: settings.tips_in_last_24_url,
                dataType: 'json',
                success: function(response) {
                    settings.tipped_performer_last_24hrs = parseInt(response.tipped_performer_last_24hrs);
                    tipping.tipped_performer_last_24hrs = settings.tipped_performer_last_24hrs;
                    tipping.check_rating_ctrl();
                }
            });
        }

        function get_contextual_menu_html(domele) {
            if (window.isMobileSite) {
                return;
            }
            var h = $("<div class='jscontextMenu' style='display: none;'></div>");
            var nick = $(domele).attr('data-nick');
            var jid = nick;
            var account_name = settings.handler.get_account_name(jid);
            var target_is_mod = false;
            var source_is_mod = false;
            h.append("<p class=\"jscontextLabel\"><img src=\"" + settings.media_url + "images/red-arrow-down.gif\" width=\"" + "9\" height=\"5\" alt=\"\" />" + nick + "</p><hr>");
            h.append("<div class=\"submenu_profile\" style=\"padding-left:8px\"> Loading...</div>");
            $.ajax({
                url: settings.get_chat_user_menu_url,
                data: {
                    fromuser: settings.muc_nickname,
                    touser: nick,
                    roomname: settings.handler.room_owner_nick
                },
                type: 'POST',
                success: function(data, textStatus, xhr) {
                    h.children().remove();
                    if (!xhr.getResponseHeader('X-Successful')) {
                        h.append("<p class=\"jscontextLabel\"><img src=\"" +
                            settings.media_url + "images/red-arrow-down.gif\" width=\"" + "9\" height=\"5\" alt=\"\" />" + nick + "</p><hr>");
                        h.append("<div class=\"submenu_profile\" " + "style=\"padding-left:8px\">An error occurred.</div>");
                        return;
                    }
                    if (xhr.getResponseHeader('X-Source_Is_Mod')) {
                        source_is_mod = true;
                    }
                    if (xhr.getResponseHeader('X-Target_Is_Mod')) {
                        target_is_mod = true;
                    }

                    function get_header_int_value(xhr, header_key) {
                        var header = xhr.getResponseHeader(header_key);
                        if (header) {
                            return parseInt(header);
                        }
                        return null;
                    }

                    function get_tip_display(tip_amount) {
                        if (tip_amount && tip_amount > 0) {
                            return tip_amount.toString();
                        }
                        return "0";
                    }

                    function add_link(cls, text, callback) {
                        var line = $("<p class='jscontextBtn " + cls + "' data-nick='" + nick + "'>" + text + "</p>");
                        line.click(function() {
                            jscontext('close');
                        });
                        line.click(callback);
                        h.append(line);
                    }
                    h.append("<p class=\"jscontextLabel\"><img src=\"" + settings.media_url + "images/red-arrow-down.gif\" width=\"" + "9\" height=\"5\" alt=\"\" />" + nick + "</p><hr>");
                    h.append("<div class=\"submenu_profile\"></div><hr>");
                    if (nick != settings.muc_nickname) {
                        add_link("pm", gettext("Send private message"), function() {
                            tabs.open_pm(nick);
                        });
                        if (settings.handler.user_is_ignored(jid)) {
                            add_link("unignore", gettext("Unignore This User"), function(event) {
                                var ignorenick = $(event.target).attr('data-nick');
                                public_methods.unignore_user(ignorenick);
                                tabs._show_chat_tab();
                            });
                        } else {
                            add_link("ignore", gettext("Ignore This User"), function(event) {
                                var ignorenick = $(event.target).attr('data-nick');
                                settings.handler.ignore(ignorenick);
                                tabs._show_chat_tab();
                            });
                        }
                        if (source_is_mod) {
                            if (!target_is_mod && nick != settings.handler.room_owner_nick) {
                                h.append("<hr>");
                                add_link("", gettext("Silence for 6 hours"), function(event) {
                                    var bannick = $(event.target).attr('data-nick');
                                    var jid = bannick;
                                    var msg = interpolate(gettext("Silence %(username)s?"), {
                                        username: bannick
                                    }, true);
                                    if (confirm(msg)) {
                                        msg = interpolate(gettext("Silencing %(username)s"), {
                                            username: bannick
                                        }, true);
                                        add_system_message(msg, tabs.get_current_tab_domele());
                                        var account_name = settings.handler.get_account_name(jid);
                                        var url = settings.silence_url.replace('USERNAMESLUG', account_name);
                                        $.post(url, {
                                            'foo': 'bar'
                                        });
                                    }
                                });
                            }
                            if (settings.muc_nickname === settings.handler.room_owner_nick) {
                                add_link("", gettext("Kick/Ban"), function(event) {
                                    var bannick = $(event.target).attr('data-nick');
                                    var msg = interpolate(gettext("Ban %(username)s?"), {
                                        username: bannick
                                    }, true);
                                    if (confirm(msg)) {
                                        msg = interpolate(gettext("Kicking and banning %(username)s"), {
                                            username: bannick
                                        }, true);
                                        add_system_message(msg, tabs.get_current_tab_domele());
                                        settings.handler.message_outbound.send_kickban_user(bannick);
                                        var account_name = settings.handler.get_account_name(bannick);
                                        var url = settings.ban_url.replace('USERNAMESLUG', account_name);
                                        $.post(url, {
                                            'foo': 'bar'
                                        });
                                    }
                                });
                                h.append("<hr>");
                                if (!target_is_mod) {
                                    add_link("", gettext("Promote to Moderator"), function(event) {
                                        public_methods.promote_user_to_moderator(nick);
                                    });
                                } else {
                                    add_link("", gettext("Remove Moderator Status"), function(event) {
                                        public_methods.revoke_moderator_status_from_user(nick);
                                    });
                                }
                            }
                        }
                    }
                    var tipped_performer_last_hour = get_header_int_value(xhr, 'X-Tipped_Performer_Last_Hour');
                    var tipped_performer_last_24_hours = get_header_int_value(xhr, 'X-Tipped_Performer_Last_24_Hours');
                    var tipped_performer_last_two_weeks = get_header_int_value(xhr, 'X-Tipped_Performer_Last_Two_Weeks');
                    if (settings.is_broadcasting && settings.muc_nickname != nick) {
                        h.append("<hr>");
                        h.append("<div class=\"jscontextLabel\" style=\"margin:0px 5px\"><img src=\"" + settings.media_url + "images/ico-tip.gif\" width=\"" + "9\" height=\"9\" alt=\"\" />" + nick + " has tipped me:</div>");
                        h.append("<div style=\"padding-left:19px\">" + get_tip_display(tipped_performer_last_hour) + " in the past hour</div>");
                        h.append("<div style=\"padding-left:19px\">" + get_tip_display(tipped_performer_last_24_hours) + " in the past day</div>");
                        h.append("<div style=\"padding-left:19px\">" + get_tip_display(tipped_performer_last_two_weeks) + " in the past 2 weeks</div>");
                    }
                    var url = settings.submenu_profile_url.replace('USERNAMESLUG', account_name);
                    consolelog("Loading menu from " + url);
                    h.find(".submenu_profile").load(url);
                    h.fadeIn(200);
                    if ($(window).height() - h.offset().top + $(window).scrollTop() < 160) {
                        $('.jscontextContainer .jscontextMenu').css('margin-top', -130);
                    }
                }
            });
            return h;
        }

        function add_system_message(message, domele, index) {
            if (index === undefined) {
                index = 0;
            }
            message_receiver.add_message('<div id="' + index + '" class="text">' + '<p>' + message + '</p></div>', domele, index);
        }
        $.add_system_message = add_system_message;

        function insert_user(newhtml, nick, token_balance) {
            var sort_key = tabs.get_sort_users_key();
            var inserted = false;
            settings.userslist.find("div.text").each(function(index, domEle) {
                var $domEle = $(domEle);
                var domnick = String($domEle.attr('data-nick'));
                if (sort_key == 'a') {
                    var new_val = nick.replace('__anonymous__', 'zzzz');
                    var current_val = domnick.replace('__anonymous__', 'zzzz');
                } else {
                    var new_val = parseInt(token_balance);
                    var current_val = parseInt($domEle.data('tokenbalance'));
                }
                if ((sort_key == 'a') && (new_val < current_val)) {
                    $domEle.before(newhtml);
                    inserted = true;
                    return false;
                }
                if ((sort_key == 't') && (new_val > current_val)) {
                    $domEle.before(newhtml);
                    inserted = true;
                    return false;
                }
            });
            if (!inserted) {
                settings.userslist.append(newhtml);
            }
        }

        function facebox_revealed() {
            var windowHeight = $(window).height();
            var faceboxHeight = $('#facebox').height();
            if (faceboxHeight < windowHeight) {
                $('#facebox').css('top', (Math.floor((windowHeight - faceboxHeight) / 2) + $(window).scrollTop()));
            }
        }
        $(document).bind('afterReveal.facebox', facebox_revealed);
        var public_methods = {
            clear_app: function() {
                panel.clear_app();
            },
            apperrorlog: function(msg) {
                if (settings.debug_mode | settings.is_broadcasting) {
                    add_system_message("Error: " + msg);
                }
            },
            appnotice: function(msg, background, foreground, weight) {
                var back_color = message_receiver._validate_hex_color(background, 'transparent');
                var fore_color = message_receiver._validate_hex_color(foreground, '#000000');
                var font_weight = message_receiver._validate_font_weight(weight, 'normal');
                message_receiver.add_message('<div class="text"><p><span style="background-color: ' +
                    back_color + '; color: ' + fore_color + '; font-weight: ' + font_weight + '">' + "Notice: " + message_receiver._add_emoticons(msg) + '</span></p></div>');
            },
            appprivatenotice: function(msg, to_user, background, foreground, weight) {
                if (to_user == settings.my_username) {
                    var back_color = message_receiver._validate_hex_color(background, 'transparent');
                    var fore_color = message_receiver._validate_hex_color(foreground, '#000000');
                    var font_weight = message_receiver._validate_font_weight(weight, 'normal');
                    message_receiver.add_message('<div class="text"><p><span style="background-color: ' +
                        back_color + '; color: ' + fore_color + '; font-weight: ' + font_weight + '">' + "Notice: " + message_receiver._add_emoticons(msg) + '</span></p></div>');
                }
            },
            apphiddenapprove: function(initial_hide_cam) {
                scheduleHiddenShowReloadForHls(initial_hide_cam);
            },
            debuglog: function(msg) {
                if (settings.debug_mode) {
                    add_system_message("Debug: " + msg);
                }
            },
            update_panel: function() {
                panel.update();
            },
            app_tab_refresh: function() {
                apps_and_bots.refresh_panel();
                panel.update();
            },
            spy_on_private: function() {
                groups_and_privates.start_spy_on_private_clicked();
            },
            exit_away_mode: function() {
                groups_and_privates.return_from_away_mode_clicked();
            },
            join_group_show: function() {
                groups_and_privates.start_group_show_clicked();
            },
            change_title: function(title) {
                if (title == settings.current_subject) {
                    return;
                }
                $.cookie('my_room_subject', title, {
                    expires: 365
                });
                settings.handler.message_outbound.send_change_title(title);
            },
            select_emoticon: function(emoticon) {
                settings.textinput.val(settings.textinput.val() + " " + emoticon);
            },
            is_goal_set: function() {
                return settings.room_goal.hasOwnProperty('amount');
            },
            increment_public_tab_counter: function() {
                if (tabs.current_tab != 'chat') {
                    var counter = $('div.chat-box span.counter');
                    var value = parseInt(counter.find('span.val').text());
                    value += 1;
                    counter.find('.val').text(value);
                    counter.show();
                }
            },
            do_get_or_create_pm_tab_for_nick: function(nick, show_alert) {
                return tabs.get_or_create_pm_tab_for_nick(nick, show_alert);
            },
            do_private_message_received: function(nick) {
                tabs.private_message_received(nick);
            },
            do_reset_users_list_html: function() {
                settings.userslist.html("");
            },
            do_update_token_balance: function(new_balance) {
                tipping.current_balance = new_balance;
                $('.tokencount').html(tipping.current_balance);
                update_tips_in_last_24();
            },
            do_show_reconnect_message: function() {
                add_system_message(gettext("trying to reconnect"));
            },
            do_show_system_message: function(message) {
                add_system_message(message, tabs.get_current_tab_domele());
            },
            do_show_kicked_message: function(reason) {
                switch (reason) {
                    case "rejoined":
                        add_system_message("You were kicked because you have joined " + "this room again.", tabs.get_current_tab_domele());
                        break;
                    case "kicked":
                        add_system_message("The broadcaster has kicked you " + "from the room.", tabs.get_current_tab_domele());
                        break;
                    case 'password':
                        add_system_message('Chat disconnected. The broadcaster has set a new password on this room.', tabs.get_current_tab_domele());
                        break;
                    default:
                        add_system_message("You were kicked from the room.", tabs.get_current_tab_domele());
                        break;
                }
            },
            promote_user_to_moderator: function(nick) {
                $.post(settings.add_moderator_url.replace('USERNAMESLUG', nick), {
                    'foo': 'bar'
                });
            },
            revoke_moderator_status_from_user: function(nick) {
                var url = settings.remove_moderator_url.replace('USERNAMESLUG', nick);
                $.post(url, {
                    'foo': 'bar'
                });
                tabs._show_chat_tab();
            },
            do_announce_promote: function(from_user, to_user, index) {
                if (to_user == settings.username) {
                    settings.mod_status_changed = true;
                }
                if (index === undefined) {
                    index = 0;
                }
                var msg = interpolate(gettext("%(from)s has granted moderator privileges to %(to)s."), {
                    from: from_user,
                    to: to_user
                }, true);
                add_system_message(msg, null, index);
            },
            do_announce_revoke: function(from_user, to_user, index) {
                if (to_user == settings.username) {
                    settings.mod_status_changed = true;
                }
                if (index === undefined) {
                    index = 0;
                }
                var msg = interpolate(gettext("%(from)s has revoked moderator privileges from %(to)s."), {
                    from: from_user,
                    to: to_user
                }, true);
                add_system_message(msg, null, index);
            },
            do_get_current_tab: function() {
                return tabs.get_current_tab_domele()
            },
            update_is_private_show_owner: function(val) {
                settings.is_private_show_owner = val;
            },
            update_viewer_count: function(amount) {
                settings.usercount.text(amount);
            },
            scroll_to_bottom: function() {
                tabs._scroll_to_bottom();
            },
            enable_html5_video_controls: function() {
                if (!HasFlash() || (navigator.userAgent.toLowerCase().indexOf('safari') > -1)) {
                    if (!settings.domroot.find(".tip_popup").is(":visible") && !$("#report_popup").is(":visible")) {
                        $("#still_video_object").attr("controls", "controls");
                        var isiPhone = navigator.userAgent.toLowerCase().indexOf("iphone");
                        var isiPad = navigator.userAgent.toLowerCase().indexOf("ipad");
                        var isFirefox = navigator.userAgent.toLowerCase().indexOf("firefox");
                        var isChrome = navigator.userAgent.toLowerCase().indexOf("chrome");
                        var isAndroid = navigator.userAgent.toLowerCase().indexOf("android");
                        var isIE = isAnyIE();
                        if (isiPhone != -1) {
                            $("#still_video_object").show();
                        }
                        if ((isiPhone < 0) && (isiPad < 0) && (isFirefox < 0) && (isChrome < 0) && (!isIE) && (isAndroid < 0)) {
                            audio_volume_persist_handle();
                        }
                    }
                }
            },
            disable_html5_video_controls: function() {
                $("#still_video_object").removeAttr("controls");
                if (navigator.platform.indexOf("iPhone") != -1) {
                    $("#still_video_object").hide();
                }
            },
            room_entry: function(username, is_mod, in_fanclub, tipped_recently, has_tokens, tipped_alot_recently, tipped_tons_recently) {
                if (tipped_alot_recently === undefined)
                    tipped_alot_recently = false;
                if (tipped_tons_recently === undefined)
                    tipped_tons_recently = false;
                if (settings.room_entry_for == 'none' && (settings.is_broadcasting || settings.is_moderator)) {
                    return;
                }
                if (settings.room_entry_for == 'orgb' || settings.room_entry_for == undefined || (!settings.is_broadcasting && !settings.is_moderator) || ((settings.room_entry_for == 'org' && username === settings.handler.room_owner_nick) || (settings.room_entry_for == 'org' && is_mod) || (settings.room_entry_for == 'org' && in_fanclub))) {
                    var label_class = message_receiver.get_username_class(username, is_mod, in_fanclub, tipped_recently, has_tokens, tipped_alot_recently, tipped_tons_recently);
                    var prefix = message_receiver.get_username_prefix(username, is_mod, in_fanclub);
                    add_system_message(prefix + '<span class="username othermessagelabel ' + label_class + '" ' + 'data-nick="' + username + '">' + username + '</span> ' + ' has joined the room.');
                }
            },
            room_leave: function(username, is_mod, in_fanclub, tipped_recently, has_tokens, tipped_alot_recently, tipped_tons_recently) {
                if (tipped_alot_recently === undefined)
                    tipped_alot_recently = false;
                if (tipped_tons_recently === undefined)
                    tipped_tons_recently = false;
                if (settings.room_leave_for == 'none' && (settings.is_broadcasting || settings.is_moderator)) {
                    return;
                }
                if (settings.room_leave_for == 'orgb' || settings.room_leave_for == undefined || (!settings.is_broadcasting && !settings.is_moderator) || ((settings.room_leave_for == 'org' && username === settings.handler.room_owner_nick) || (settings.room_leave_for == 'org' && is_mod) || (settings.room_leave_for == 'org' && in_fanclub))) {
                    var label_class = message_receiver.get_username_class(username, is_mod, in_fanclub, tipped_recently, has_tokens, tipped_alot_recently, tipped_tons_recently);
                    var prefix = message_receiver.get_username_prefix(username, is_mod, in_fanclub);
                    add_system_message(prefix + '<span class="username othermessagelabel ' + label_class + '" ' + 'data-nick="' + username + '">' + username + '</span> ' + ' has left the room.');
                }
            },
            unignore_user: function(username) {
                $('#ignore_user_form_username').val(username);
                $('#ignore_user_form_remove').val('1');
                settings.handler.unignore(username);
                tabs._show_chat_tab();
            },
            scroll_chat_into_view: function() {
                if (settings.scroll_on_load && settings.textinput.offset() != null) {
                    $(document).scrollTop(settings.textinput.offset().top - $(window).height() + settings.textinput.outerHeight() + 12);

                    function isScrolledIntoView(elem) {
                        var docViewTop = $(window).scrollTop();
                        var docViewBottom = docViewTop + $(window).height();
                        var elemTop = $(elem).offset().top;
                        var elemBottom = elemTop + $(elem).height();
                        return ((elemBottom >= docViewTop) && (elemTop <= docViewBottom));
                    }
                    if ($('.top-section').length > 0) {
                        if (!isScrolledIntoView($('.top-section'))) {
                            $(document).scrollTop($('.top-section').offset().top);
                        }
                    }
                    save_scrolling();
                }
            }
        };

        function remove_user_from_list(nick) {
            settings.userslist.find("[data-nick='" + nick + "']").remove();
        }
        $(document).bind("disconnected.defchat", function() {
            add_system_message(gettext("chat disconnected"));
            consolelog("Trying to re connect to " + settings.chat_host + " with " +
                settings.user + ":" + settings.password);
            public_methods.do_show_reconnect_message();
        });
        $(document).bind("connectionfailed.defchat", function() {
            add_system_message(gettext("unable to connect"));
        });
        $(document).bind("connecting.defchat", function() {
            add_system_message(gettext("connecting"));
        });

        function htmlDecode(value) {
            return $('<div/>').html(value).text();
        }

        function private_show_owner_connected() {
            $.post(settings.is_private_show_owner_url, {
                'foo': 'bar'
            }, function(data) {
                var response_data = {
                    is_private_show_owner: 0
                };
                try {
                    response_data = $.parseJSON(data);
                } catch (e) {}
                settings.is_private_show_owner = response_data.is_private_show_owner;
                settings.handler.initializer.is_on_private_room = Boolean(settings.is_private_show_owner);
            }).always(function() {
                connected_post_process();
            });
        }

        function connected_post_process() {
            if (settings.welcome_message) {
                add_system_message(settings.welcome_message);
            }
            if (settings.welcome_warning) {
                add_system_message(settings.welcome_warning);
            }
            if (settings.app_info_json) {
                var links = [];
                try {
                    var msginfo = $.parseJSON(settings.app_info_json);
                    for (var i = 0; i < msginfo.length; i++) {
                        var link = $('<a />');
                        link.text(msginfo[i][0]);
                        link.attr('target', "_blank");
                        if (window.isMobileSite) {
                            link.attr('href', msginfo[i][0]);
                        } else {
                            link.attr('href', msginfo[i][1]);
                        }
                        links.push(link[0].outerHTML);
                    }
                } catch (e) {
                    return;
                }
                if (links.length > 0) {
                    if (settings.is_broadcasting) {
                        add_system_message("You are running these apps: " +
                            links.join(", "));
                    } else {
                        add_system_message("Broadcaster " + settings.handler.room_owner_nick + " is running these apps: " + links.join(", "));
                    }
                }
            }
            if (settings.num_users_waiting_for_group_show > 0 && settings.num_users_required_for_group_show > 0) {
                groups_and_privates.show_group_show_waiting_message(settings.num_users_waiting_for_group_show, settings.num_users_required_for_group_show)
            }
            $(window).unload(function() {
                settings.handler.disconnect();
            });
        }

        function connected() {
            if (settings.is_private_show_owner) {
                private_show_owner_connected();
            } else {
                connected_post_process();
            }
        }
        $(document).bind("connected.defchat", connected);
        var tabs = {
            pm_tab_visible: false,
            current_tab: 'chat',
            pm_tab: null,
            pm_tab_map: {},
            settings_body: null,
            unread_pm_map: {},
            total_unread_pms: 0,
            current_pm_nick: null,
            opened_pms: new Array(),
            is_blinking: false,
            open_pm: function(nick) {
                tabs._show_pm_tab();
                tabs._remove_unread_pms_for_nick(nick);
                settings.domroot.find('.chat-box > div').hide();
                tabs.get_or_create_pm_tab_for_nick(nick, false).show();
                tabs.current_tab = 'pm';
                tabs.current_pm_nick = nick;
                tabs._enable_form();
                tabs._scroll_to_bottom();
            },
            after_close_pm: function(nick) {},
            close_pm: function(nick) {
                tabs._show_pm_tab();
                tabs._remove_unread_pms_for_nick(nick);
                settings.domroot.find('.chat-box > div').hide();
                tabs.pm_tab_map[nick].remove();
                tabs.pm_tab_map[nick] = null;
                tabs._show_chat_tab();
                var newopens = new Array();
                for (var i = 0; i < tabs.opened_pms.length; i++) {
                    if (tabs.opened_pms[i] != nick) {
                        newopens.push(tabs.opened_pms[i]);
                    }
                }
                tabs.opened_pms = newopens;
                tabs.after_close_pm(nick);
            },
            refresh_userlist: function() {
                var sort_key = tabs.get_sort_users_key();
                var newhtml = $("<div class='text' data-nick=''>" + "<class='hostmessagelabel'>Loading user list...</class></div>");
                public_methods.do_reset_users_list_html();
                settings.userslist.append(newhtml);
                $('#get_chat_user_list_form_room_sort_by').val(sort_key);
                $('#get_chat_user_list_form_room_roomname').val(settings.handler.room_owner_nick);
                $('#get_chat_user_list_form_room_private').val(settings.handler.initializer.is_on_private_room);
                $.ajax({
                    url: $('#get_chat_user_list_form').attr('action'),
                    data: $('#get_chat_user_list_form').serialize(),
                    type: 'POST',
                    success: function(data, textStatus, xhr) {
                        if (!xhr.getResponseHeader('X-Successful')) {
                            alert("An error occurred");
                            return;
                        }
                        public_methods.do_reset_users_list_html();
                        var newhtml = $("<div class='text' data-nick='' " + "style='float:right'> <a href=\"#\">" + "refresh userlist</a></div>");
                        newhtml.bind('click', tabs.refresh_userlist);
                        settings.userslist.append(newhtml);
                        var results = data.split(',');
                        for (var i in results) {
                            if (i > 0) {
                                var results_split = results[i].split('|');
                                var name = results_split[0];
                                var color = results_split[1];
                                var newhtml = $("<div class='text' data-nick='" +
                                    name + "'><strong class='color-" +
                                    color + "'>" + name + "</strong></div>");
                                newhtml.jscontext({
                                    html: get_contextual_menu_html,
                                    bind: 'anyclick'
                                });
                                settings.userslist.append(newhtml);
                            }
                        }
                        $.mydefchatconn('update_viewer_count', parseInt(results[0]) + results.length - 1);
                        if (results[0] > 0) {
                            var newhtml = $("<div class='text' data-nick=''>" + "<class='hostmessagelabel'>+" +
                                results[0] + " anonymous" +
                                (results[0] == 1 ? " user" : " users") + "</class></div>");
                            settings.userslist.append(newhtml);
                        }
                    }
                });
            },
            on_tab_click: function() {
                var tab = $(this).data("tab");
                tabs.before_tab_click();
                settings.domroot.find('.chat-box > div').hide();
                if (tab == 'users') {
                    tabs._deactivate_tabs();
                    $(this).closest('li').addClass('active');
                    settings.domroot.find('.chat-box > div.users-list').show();
                    tabs.current_tab = 'users';
                    tabs._disable_form();
                    tabs.refresh_userlist();
                } else if (tab == 'chat') {
                    tabs._show_chat_tab();
                } else if (tab == 'settings') {
                    tabs._show_settings_tab();
                    tabs._disable_form();
                } else {
                    consolelog("Unknown tab " + tab);
                }
                tabs.after_tab_click();
                return false;
            },
            before_tab_click: function(tab) {},
            after_tab_click: function(tab) {},
            get_or_create_pm_tab_for_nick: function(nick, show_alert) {
                if (tabs.pm_tab_map[nick]) {
                    return tabs.pm_tab_map[nick];
                }
                if (show_alert) {
                    var msg = interpolate(gettext(("New private conversation from <span class='username' data-nick='%(username)s'>%(username)s" + "</span> <span class='pmalert'>(Press tab to cycle through your conversations or click the PM tab)" + "</span>")), {
                        username: nick
                    }, true);
                    add_system_message(msg);
                }
                tabs.opened_pms.push(nick);
                tabs.opened_pms.sort();
                tabs._create_pm_tab();
                if (tabs.opened_pms.length > 1) {
                    tabs._set_pm_tab_jscontext();
                } else {
                    tabs._set_pm_tab_direct(nick);
                }
                tabs.pm_tab_map[nick] = $("<div class='chat-list' style='display: none;'></div>");
                settings.domroot.find('.chat-box > div').last().after(tabs.pm_tab_map[nick]);
                msg = interpolate(gettext("Caution: %(sitename)s staff will NEVER contact you via chat or ask for your password. <br/><br/>" + "Private conversation with %(username)s"), {
                    username: nick,
                    sitename: settings.site_name,
                    supportemail: settings.support_email
                }, true);
                add_system_message(msg, tabs.pm_tab_map[nick]);
                return tabs.pm_tab_map[nick];
            },
            after_private_message_received: function() {},
            private_message_received: function(nick) {
                if (window.isMobileSite) {
                    return;
                }
                if (tabs.current_tab === 'pm' && tabs.current_pm_nick == nick) {
                    return;
                }
                if (tabs.unread_pm_map[nick]) {
                    tabs.unread_pm_map[nick] += 1;
                } else {
                    tabs.unread_pm_map[nick] = 1;
                }
                tabs.total_unread_pms += 1;
                tabs._refresh_pm_tab_count();
                if (!tabs.pm_tab.hasClass('active')) {
                    if (!tabs.is_blinking) {
                        tabs._blink_pm(6, 250);
                    }
                }
                tabs.after_private_message_received();
            },
            _blink_pm: function(numblinks, timeout) {
                if (tabs.current_tab === 'pm') {
                    tabs.is_blinking = false;
                    return;
                }
                if (numblinks == 0) {
                    tabs.pm_tab.removeClass('active');
                    tabs.is_blinking = false;
                    return;
                }
                if (!tabs.pm_tab.hasClass('active')) {
                    tabs.pm_tab.addClass('active');
                } else {
                    tabs.pm_tab.removeClass('active');
                }
                tabs.is_blinking = true;
                setTimeout(function() {
                    tabs._blink_pm(numblinks - 1, timeout);
                }, timeout);
            },
            get_current_tab_domele: function() {
                var r = settings.domroot.find('div.chat-list:visible');
                if (r.length === 0) {
                    return settings.chatlist;
                } else {
                    return r;
                }
            },
            _show_chat_tab: function() {
                tabs._deactivate_tabs();
                settings.buttons.find('li:first').addClass('active');
                settings.domroot.find('.chat-box > div').hide();
                settings.domroot.find('.chat-box > div.chat-list:first').show();
                var counter = $('div.chat-box span.counter');
                counter.find('span.val').text('0');
                counter.hide();
                settings.domroot.find('> div.chat-box span.counter').hide();
                tabs.current_tab = 'chat';
                tabs._enable_form();
                tabs._scroll_to_bottom();
            },
            _scroll_to_bottom: function() {
                var chat = tabs.get_current_tab_domele().get(0);
                $(".chat-list").height($("#player").height() + 32);
                if (chat !== undefined) {
                    chat.scrollTop = chat.scrollHeight;
                }
            },
            _disable_form: function() {
                settings.textinput.blur();
                settings.textinput.focus(function(event) {
                    settings.textinput.unbind(event);
                    tabs._show_chat_tab();
                });
            },
            _enable_form: function() {
                settings.textinput.unbind('focus');
                settings.textinput.focus(save_scrolling);
                settings.textinput.focusout(load_scrolling);
            },
            _remove_unread_pms_for_nick: function(nick) {
                if (tabs.unread_pm_map[nick] && tabs.unread_pm_map[nick] > 0) {
                    tabs.total_unread_pms = tabs.total_unread_pms - tabs.unread_pm_map[nick];
                    tabs.unread_pm_map[nick] = 0;
                    tabs._refresh_pm_tab_count();
                }
            },
            _refresh_pm_tab_count: function() {
                var str;
                if (tabs.total_unread_pms == 0) {
                    str = "";
                } else {
                    str = "(" + tabs.total_unread_pms + ")";
                }
                tabs.pm_tab.find('.pm_count').text(str);
            },
            _create_pm_tab: function() {
                if (!tabs.pm_tab_visible) {
                    tabs.pm_tab_visible = true;
                    tabs.pm_tab = $("<li><a href='#' data-tab='pm'>" + "PM <span class='pm_count'></span></a></li>");
                    settings.buttons.find('li:last').before(tabs.pm_tab);
                }
            },
            _set_pm_tab_direct: function(nick) {
                tabs.pm_tab.unbind('click');
                tabs.pm_tab.bind('click', function() {
                    tabs.open_pm(nick);
                    return false;
                });
            },
            _set_pm_tab_jscontext: function() {
                tabs.pm_tab.unbind('click');
                tabs.pm_tab.jscontext({
                    html: function() {
                        var h = $("<div class='jscontextMenu'></div>");

                        function add_link(nick, text, callback) {
                            var line = $("<div class=\"pmline\"><div class=\"user\" data-nick='" + nick + "'>" + text + "</div><div class=\"close\" data-nick='" + nick + "'></div></div>");
                            line.find("div.user").click(callback);
                            line.find("div.user").click(function() {
                                jscontext('close');
                            });
                            line.find("div.close").click(function(event) {
                                tabs.close_pm($(event.target).attr('data-nick'));
                                jscontext('close');
                            });
                            h.append(line);
                        }
                        h.append("<p class=\"jscontextLabel\"><img src=\"" + settings.media_url + "images/orange-arrow-down.gif\" width=\"9\" height=\"5\" alt=\"\" />Private Message with:</p><hr>");
                        for (var i = 0; i < tabs.opened_pms.length; i++) {
                            var nick = tabs.opened_pms[i];
                            var unread = tabs.unread_pm_map[nick];
                            if (unread && unread > 0) {
                                unread = " (" + unread + ")";
                            } else {
                                unread = "";
                            }
                            add_link(nick, nick + unread, function(event) {
                                var nick = $(event.target).data('nick');
                                tabs.open_pm(nick);
                            });
                        }
                        return h;
                    },
                    bind: "click"
                });
            },
            _deactivate_tabs: function() {
                settings.buttons.find('li').removeClass('active');
            },
            _show_pm_tab: function() {
                tabs._deactivate_tabs();
                tabs._create_pm_tab();
                tabs.pm_tab.addClass('active');
            },
            _sort_users_list: function() {
                var sort_key = tabs.get_sort_users_key();
                if (sort_key == 'a') {
                    var mylist = $('div.users-list');
                    var listitems = mylist.children('div.text').get();
                    listitems.sort(function(a, b) {
                        var compA = String($(a).data("nick"));
                        compA = compA.replace('__anonymous__', 'zzzz');
                        var compB = String($(b).data("nick"));
                        compB = compB.replace('__anonymous__', 'zzzz');
                        return (compA < compB) ? -1 : (compA > compB) ? 1 : 0;
                    });
                    $.each(listitems, function(idx, itm) {
                        mylist.append(itm);
                    })
                } else {
                    var mylist = $('div.users-list');
                    var listitems = mylist.children('div.text').get();
                    listitems.sort(function(a, b) {
                        var compA = parseInt($(a).data("tokenbalance"));
                        var compB = parseInt($(b).data("tokenbalance"));
                        return (compA < compB) ? 1 : (compA > compB) ? -1 : 0;
                    });
                    $.each(listitems, function(idx, itm) {
                        mylist.append(itm);
                    })
                }
            },
            apply_font_size: function() {
                if (!window.isMobileSite) {
                    $("div.chat-list").css("font-size", settings.font_size);
                }
            },
            get_sort_users_key: function() {
                if (!settings.is_broadcasting) {
                    return 'a';
                }
                return settings.sort_users_key;
            },
            _submit_settings: function() {
                $.ajax({
                    type: "POST",
                    url: settings.chat_settings_url,
                    data: $("#chat_settings").serialize(),
                    success: function(response) {
                        if (response === "supporter") {
                            alert("You must be a supporter to change this.");
                            $("#chat_color_id").val("#494949");
                            $('option:selected', "#id_font_family").removeAttr('selected');
                            $("#id_font_family").find('option[value="default"]').attr('selected', true);
                            return;
                        }
                        try {
                            var data = $.parseJSON(response);
                        } catch (e) {
                            alert("An error occurred.");
                            return;
                        }
                        display_notice('Chat settings saved');
                        settings.font_size = data.font_size;
                        settings.show_emoticons = data.show_emoticons;
                        settings.emoticon_autocomplete_delay = data.emoticon_autocomplete_delay;
                        settings.sort_users_key = data.sort_users_key;
                        settings.room_entry_for = data.room_entry_for;
                        settings.room_leave_for = data.room_leave_for;
                        settings.handler.update_enter_leave_settings(data.room_entry_for, data.room_leave_for);
                        settings.silence_broadcasters = data.silence_broadcasters;
                        settings.b_tip_vol = data.b_tip_vol;
                        settings.v_tip_vol = data.v_tip_vol;
                        tabs.apply_font_size();
                        if (settings.is_broadcasting) {
                            SetTipVolume(settings.b_tip_vol);
                            if ($("#tip_slider").val() != settings.b_tip_vol) {
                                $("#tip_slider").simpleSlider("setValue", settings.b_tip_vol, false);
                            }
                            $("#tip_slider_label").text("Tip Volume (" + settings.b_tip_vol + "%)");
                        } else {
                            SetTipVolume(settings.v_tip_vol);
                            if ($("#tip_slider").val() != settings.v_tip_vol) {
                                $("#tip_slider").simpleSlider("setValue", settings.v_tip_vol, false);
                            }
                            $("#tip_slider_label").text("Tip Volume (" + settings.v_tip_vol + "%)");
                        }
                        $("#id_broadcaster_tip_volume").val(settings.b_tip_vol);
                        $("#id_viewer_tip_volume").val(settings.v_tip_vol);
                        $("#id_v").val(settings.v_tip_vol);
                    },
                    error: function() {
                        alert("An error occurred.");
                    }
                });
            },
            _show_settings_tab: function() {
                tabs.current_tab = 'settings';
                settings.domroot.find('.chat-box > div').hide();
                if (!tabs.settings_body || settings.mod_status_changed) {
                    $(".settings-list").remove();
                    tabs.settings_body = $("<div class='settings-list'></div>");
                    tabs.settings_body.height($('.chat-list').height());
                    tabs.settings_body.load(settings.chat_settings_url, function() {
                        settings.mod_status_changed = false;
                        if (defchat_settings.is_age_verified == 'false') {
                            var allowed_users_dropdown = $('#id_allowed_chat');
                            allowed_users_dropdown.parent().attr('title', 'Only age verified broadcasters can change this setting.');
                            allowed_users_dropdown.prev().text(allowed_users_dropdown.prev().text() + ' (Age Verifcation Required)').css('color', '#888');
                            allowed_users_dropdown.prop('disabled', true);
                            allowed_users_dropdown.after('<input type="hidden" name="allowed_chat" value="all" />');
                        }
                        $("#chat_settings").live("change", function() {
                            if (login_required()) {
                                return false;
                            } else {
                                if (settings.is_broadcasting) {
                                    if ($("#tip_slider").hasClass('is_dragging')) {
                                        settings.b_tip_vol = parseFloat($("#tip_slider").val());
                                        $("#tip_slider_label").text("Tip Volume (" + settings.b_tip_vol + "%)");
                                        $("#id_broadcaster_tip_volume").val(settings.b_tip_vol);
                                        return false;
                                    }
                                } else {
                                    if ($("#tip_slider").hasClass('is_dragging')) {
                                        settings.v_tip_vol = parseFloat($("#tip_slider").val());
                                        $("#tip_slider_label").text("Tip Volume (" + settings.v_tip_vol + "%)");
                                        $("#id_viewer_tip_volume").val(settings.v_tip_vol);
                                        return false;
                                    }
                                }
                                if (settings.is_broadcasting) {
                                    settings.b_tip_vol = parseFloat($("#tip_slider").val());
                                    $("#tip_slider_label").text("Tip Volume (" + settings.b_tip_vol + "%)");
                                    $("#id_broadcaster_tip_volume").val(settings.b_tip_vol);
                                } else {
                                    settings.v_tip_vol = parseFloat($("#tip_slider").val());
                                    $("#tip_slider_label").text("Tip Volume (" + settings.v_tip_vol + "%)");
                                    $("#id_viewer_tip_volume").val(settings.v_tip_vol);
                                }
                                setTimeout(tabs._submit_settings, 500);
                                return false;
                            }
                        });
                        $('#chat_color_id').mColorPicker();
                        $('#chat_color_id').bind('colorpicked', function() {
                            $.ajax({
                                url: $(this).parent().attr('action'),
                                data: $(this).parent().serialize(),
                                type: 'POST',
                                success: function(response) {
                                    if (response === "supporter") {
                                        alert("You must be a supporter to change this.");
                                        $("#chat_color_id").val("#494949");
                                        $('option:selected', "#id_font_family").removeAttr('selected');
                                        $("#id_font_family").find('option[value="default"]').attr('selected', true);
                                        return;
                                    }
                                    display_notice('Chat settings saved');
                                },
                                failure: function() {
                                    alert('An error occurred.');
                                }
                            });
                        });
                        $("#ignored_users_id").click(function() {
                            if (login_required()) {
                                return false;
                            }
                            jQuery.facebox({
                                ajax: settings.chat_ignore_list_url
                            });
                        });
                        $("#mod_list_link").click(function() {
                            if (login_required()) {
                                return false;
                            }
                            jQuery.facebox({
                                ajax: settings.chat_mod_list_url
                            });
                        });
                        $("#fan_list_link").click(function() {
                            if (login_required()) {
                                return false;
                            }
                            jQuery.facebox({
                                ajax: settings.chat_fan_list_url
                            });
                        });
                        $("#ban_list_link").click(function() {
                            if (login_required()) {
                                return false;
                            }
                            jQuery.facebox({
                                ajax: settings.chat_ban_list_url
                            });
                        });
                        $("#geo_list_link").click(function() {
                            if (login_required()) {
                                return false;
                            }
                            jQuery.facebox({
                                ajax: settings.chat_geo_list_url
                            });
                        });
                    });
                    settings.domroot.find('.chat-box > div').last().after(tabs.settings_body);
                }
                tabs.settings_body.show();
                tabs._deactivate_tabs();
                settings.buttons.find('a[data-tab="settings"]').parent().addClass('active');
            }
        };
        settings.buttons.find('a').click(tabs.on_tab_click);
        $.chat_tabs = tabs;
        $("body").mouseup(function(e) {
            if ($("#tip_slider").hasClass('is_dragging')) {
                if (login_required()) {
                    return false;
                }
                tabs._submit_settings();
            }
        });
        var message_sender = {
            message_to_send: '',
            confirmed_send: false,
            _recent_message_log: new Array(),
            _handle_action: function() {
                var msg = settings.textinput.val();
                if (msg.charAt(0) !== '/') {
                    return false;
                }
                var params = msg.split(" ");
                var action = params.shift();
                if (action === "/tip") {
                    var amount = parseInt(params.shift());
                    var message = params.join(" ");
                    if (window.isMobileSite) {
                        showTipOverlay(amount, message);
                    } else {
                        tipping.toggle_overlay(amount, message);
                    }
                    return true;
                } else if (action == '/debug') {
                    if (settings.debug_mode) {
                        settings.debug_mode = false;
                        add_system_message("Debug mode disabled.")
                    } else {
                        settings.debug_mode = true;
                        add_system_message("Debug mode enabled. " + "Type /debug again to disable.")
                    }
                    return true;
                }
                return false;
            },
            on_form_submit: function() {
                if ($.trim(settings.textinput.val()) == "") {
                    message_sender._reset_form();
                    return false;
                }
                if (message_sender._handle_action()) {
                    message_sender._reset_form();
                    return false;
                }
                if (settings.first_post_warning !== null) {
                    if (!message_sender.confirmed_send) {
                        if (!confirm(settings.first_post_warning)) {
                            return false;
                        }
                        message_sender.confirmed_send = true;
                    }
                }
                if (!settings.handler.initializer.joined) {
                    add_system_message('not connected');
                    return false;
                }
                if (settings.username.indexOf('__anonymous__') === 0) {
                    if (settings.login_required_callback) {
                        settings.login_required_callback();
                    } else {
                        add_system_message(gettext("anonymous users cannot chat - create your free account"), tabs.get_current_tab_domele());
                    }
                    return false;
                }
                message_sender.message_to_send = settings.textinput.val();
                message_sender._reset_form();
                message_sender._translate_and_send();
                message_sender._recent_message_log.push(new Date());
                return false;
            },
            _get_recent_message_log_count: function() {
                message_sender._remove_old_messages_from_log();
                return message_sender._recent_message_log.length;
            },
            _remove_old_messages_from_log: function() {
                var now = new Date();
                message_sender._recent_message_log = $.grep(message_sender._recent_message_log, function(a) {
                    return ((now - a) < (10 * 1000));
                });
            },
            _user_has_tokens: function() {
                return (tipping.current_balance > 0 ? '1' : '0');
            },
            _translate_and_send: function() {
                session_metrics.messageCount += 1;
                message_sender._reset_form();
                if (tabs.current_tab === 'pm') {
                    settings.handler.message_outbound.send_private_message(message_sender.message_to_send, tabs.current_pm_nick);
                } else {
                    settings.handler.message_outbound.send_room_message(message_sender.message_to_send);
                }
            },
            _reset_form: function() {
                settings.textinput.val('');
                settings.textinput.blur();
                tabs._scroll_to_bottom();
            }
        };
        $message_sender = message_sender;
        var panel = {
            poll_again: false,
            is_loading: false,
            update: function() {
                if (!settings.get_panel_url) {
                    return;
                }
                if (panel.is_loading === true) {
                    panel.poll_again = true;
                    return;
                }
                panel.poll_again = false;
                panel.is_loading = true;
                $('.goal_display').first().load(settings.get_panel_url + '?_=' + (new Date()).getTime(), function(response, textStatus, jqXhr) {
                    if (response === "") {
                        if (!settings.handler.initializer.is_on_private_room && settings.handler.initializer.model_status != "away") {
                            groups_and_privates.change_broadcaster_panel('default');
                        }
                    } else {
                        $('.app_name').text(jqXhr.getResponseHeader('x-app-name'));
                        if (!settings.handler.initializer.is_on_private_room && $('.broadcaster_panel_default').is(':visible')) {
                            groups_and_privates.change_broadcaster_panel('goal_display');
                        }
                    }
                    panel.is_loading = false;
                    if (panel.poll_again) {
                        setTimeout(panel.update, 0);
                    }
                });
            },
            clear_app: function() {
                panel.update();
                public_methods.app_tab_refresh();
            },
            init: function() {
                panel.update();
            }
        };
        panel.init();
        var emoticons = {
            _emoticons_loaded: false,
            _popup_window: null,
            init: function() {
                settings.domroot.find(".emoticon_button").click(emoticons.on_emoticon_button_clicked);
            },
            on_emoticon_button_clicked: function(ev) {
                if (settings.login_required_callback) {
                    if (settings.login_required_callback()) {
                        return false;
                    }
                }
                emoticons.toggle_overlay();
            },
            on_body_clicked_while_overlay_up: function(ev) {
                if (settings.domroot.find('.emote_popup').has(ev.target).length || settings.domroot.find('.emoticon_button').is(ev.target)) {
                    return;
                }
                emoticons.toggle_overlay();
            },
            on_popup_clicked: function(ev) {
                if (!emoticons._popup_window || emoticons._popup_window.closed) {
                    emoticons._popup_window = window.open($(this).attr('href'), '_blank', 'status=0,toolbar=0,menubar=0,directories=0,resizable=1,' + 'scrollbars=1,height=615,width=850');
                    emoticons._popup_window.onload = function() {
                        emoticons._popup_window.RunCallbackFunction = emoticons.on_popup_callback;
                    };
                    $(window).unload(emoticons.on_document_closing);
                }
                emoticons.toggle_overlay();
                emoticons._popup_window.focus();
                return false;
            },
            on_document_closing: function() {
                if (emoticons._popup_window) {
                    if (!emoticons._popup_window.closed) {
                        emoticons._popup_window.close();
                    }
                }
            },
            on_popup_callback: function() {
                alert("popup callback");
            },
            on_emoticon_clicked: function() {
                emoticons.toggle_overlay();
                settings.textinput.val(settings.textinput.val() + " " +
                    $(this).data('emoticon') + " ");
            },
            toggle_overlay: function() {
                var ep = settings.domroot.find(".emote_popup");
                if (ep.is(":visible")) {
                    $("body").unbind('click', emoticons.on_body_clicked_while_overlay_up);
                    ep.hide();
                } else {
                    $("body").click(emoticons.on_body_clicked_while_overlay_up);
                    if (emoticons._emoticons_loaded) {
                        ep.show();
                    } else {
                        emoticons._emoticons_loaded = true;
                        ep.load(settings.emoticon_overlay_url, function() {
                            ep.show();
                            ep.find('.more a').click(emoticons.on_popup_clicked);
                            ep.find('img.emoticon').click(emoticons.on_emoticon_clicked);
                        });
                    }
                }
            }
        };
        emoticons.init();
        var message_receiver = {
            ignored_users: settings.load_ignored_users.split(','),
            ignore_delayed: false,
            on_settingsupdate: function(allow_privates, allow_groups, minimum_users_for_group_show, private_price, group_price, spy_price) {
                if (allow_privates === true || allow_privates === 'True') {
                    $('.private_settings .value').html(gettext("Allowed"));
                    groups_and_privates.show_private_show_controls(true);
                } else {
                    $('.private_settings .value').html(gettext("Disabled"));
                    groups_and_privates.hide_private_show_controls(true);
                }
                if (allow_groups === true || allow_groups === 'True') {
                    $('.group_show_disabled').hide();
                    $('.group_show_active').show();
                    groups_and_privates.show_group_show_controls(true);
                } else {
                    $('.group_show_active').hide();
                    $('.group_show_disabled').show();
                    groups_and_privates.hide_group_show_controls(true);
                }
                $('.num_users_required_for_group').html(minimum_users_for_group_show);
                if (private_price && private_price != settings.private_price) {
                    var msg = interpolate(gettext('%(username)s changed Private Show price to %(price)s tokens per minute'), {
                        username: settings.handler.room_owner_nick,
                        price: private_price
                    }, true);
                    add_system_message(msg, tabs.get_current_tab_domele());
                    settings.private_price = private_price;
                }
                if (group_price && group_price != settings.group_price) {
                    var msg = interpolate(gettext('%(username)s changed Group Show price to %(price)s tokens per minute'), {
                        username: settings.handler.room_owner_nick,
                        price: group_price
                    }, true);
                    add_system_message(msg, tabs.get_current_tab_domele());
                    settings.group_price = group_price;
                }
                if (spy_price && spy_price != settings.spy_price) {
                    var msg = interpolate(gettext('%(username)s changed Spy on Private Show price to %(price)s tokens per minute'), {
                        username: settings.handler.room_owner_nick,
                        price: spy_price
                    }, true);
                    add_system_message(msg, tabs.get_current_tab_domele());
                    settings.spy_price = spy_price;
                }
                return true;
            },
            get_username_class: function(username, is_mod, in_fanclub, tipped_recently, has_tokens, tipped_alot_recently, tipped_tons_recently) {
                if (tipped_alot_recently === undefined)
                    tipped_alot_recently = false;
                if (tipped_tons_recently === undefined)
                    tipped_tons_recently = false;
                if (username === settings.handler.room_owner_nick) {
                    return 'hostmessagelabel';
                } else if (is_mod) {
                    return 'moderatormessagelabel';
                } else if (in_fanclub) {
                    return 'fanclubmessagelabel';
                } else if (tipped_tons_recently) {
                    return 'tippedtonsrecentlymessagelabel';
                } else if (tipped_alot_recently) {
                    return 'tippedalotrecentlymessagelabel';
                } else if (tipped_recently) {
                    return 'tippedrecentlymessagelabel';
                } else if (has_tokens) {
                    return 'hastokensmessagelabel';
                }
                return '';
            },
            get_username_prefix: function(username, is_mod, in_fanclub) {
                if (username === settings.handler.room_owner_nick) {
                    return 'Broadcaster ';
                } else if (is_mod) {
                    return 'Moderator ';
                } else if (in_fanclub) {
                    return 'Fan Club member ';
                }
                return '';
            },
            _get_tip_sound_level: function(amount) {
                if (amount >= 1000) {
                    return 'huge';
                } else if (amount >= 500) {
                    return 'large';
                } else if (amount >= 100) {
                    return 'medium';
                } else if (amount >= 15) {
                    return 'small';
                }
                return 'tiny';
            },
            on_tipalert: function(amount, from_username, to_username, message, is_delayed, is_mod, in_fanclub, tipped_recently, has_tokens, index, tipped_alot_recently, tipped_tons_recently) {
                if (index === undefined)
                    index = 0;
                if (tipped_alot_recently === undefined)
                    tipped_alot_recently = false;
                if (tipped_tons_recently === undefined)
                    tipped_tons_recently = false;
                var msg = interpolate(gettext("tipped %(amount)s" + ((amount === 1) ? ' token' : ' tokens')), {
                    amount: amount
                }, true)
                if (settings.is_broadcasting) {
                    if (!is_delayed) {
                        if ($.trim(message) != "") {
                            msg = msg + " -- " + message;
                        }
                    }
                    $.ajax({
                        url: '/tipping/get_token_balance/',
                        dataType: 'json',
                        type: 'POST',
                        success: function(response) {
                            if (response.success) {
                                $('.tokencount').html(response.token_balance);
                            }
                        }
                    });
                }
                if (!is_delayed) {
                    PlayBeep(message_receiver._get_tip_sound_level(amount));
                }
                var label_class = message_receiver.get_username_class(from_username, is_mod, in_fanclub, tipped_recently, has_tokens, tipped_alot_recently, tipped_tons_recently);
                var msg = '<span class="tipalert">' + '<span class="username othermessagelabel ' +
                    label_class + '" ' + 'data-nick="' + from_username + '">' +
                    from_username + '</span> ' + msg + '</span>';
                add_system_message(msg, tabs.get_current_tab_domele(), index);
                if (tabs.current_tab === 'pm') {
                    add_system_message(msg, settings.chatlist);
                }
            },
            on_purchase_alert: function(msg) {
                add_system_message("<span class='purchasealert'>" +
                    msg + "</span>", tabs.get_current_tab_domele());
                update_tips_in_last_24();
            },
            on_user_banned: function(username) {
                $(".chat-list div.text [data-nick='" + username + "']").each(function(index, value) {
                    $(this).parent("div.text").remove();
                });
                var text = gettext('User') + ' ' + username + ' ';
                text += gettext('was kicked out of the room and his/her messages have been removed');
                add_system_message(text, tabs.get_current_tab_domele());
            },
            on_user_silenced: function(silenced_nick, silencer_nick, index) {
                if (index === undefined) {
                    index = 0;
                }
                $(".chat-list div.text [data-nick='" + silenced_nick + "']").each(function(index, value) {
                    $(this).parent("div.text").remove();
                });
                var text = interpolate(gettext("User %(username)s was silenced by %(silencer)s and his/her messages have been removed"), {
                    username: silenced_nick,
                    silencer: silencer_nick
                }, true);
                add_system_message(text, tabs.get_current_tab_domele(), index);
            },
            remove_messages: function(username) {
                $(".chat-list div.text:has([data-nick='" + username + "'])").remove();
            },
            on_subject_change: function(subject, show_in_chat, index) {
                if (show_in_chat == undefined) {
                    show_in_chat = '1';
                }
                if (index === undefined) {
                    index = 0;
                }
                subject = subject.replace(new RegExp('#([a-z0-9\\-_]{2,50})', 'gm'), function(x) {
                    return x.toLowerCase();
                });
                var check_tags = subject.match(/#[a-z0-9\-_]{2,50}/g);
                var checked_tags = [];
                if (check_tags !== null) {
                    for (var i = 0; i < check_tags.length; i++) {
                        checked_tags.push(check_tags[i].substr(1));
                    }
                }
                var postTitle = function(tags) {
                    var msg = interpolate(gettext("room subject changed to \"%(subject)s\""), {
                        subject: subject.replace(new RegExp('#([a-z0-9\\-_]{2,50})', 'gm'), function(x) {
                            if (tags.indexOf(x.substr(1)) !== -1) {
                                return '<a href="' + settings.hashtag_url + x.substr(1) + '/' + settings.broadcaster_gender + '">' + x + '</a>';
                            } else {
                                return x;
                            }
                        })
                    }, true);
                    if (show_in_chat === '1') {
                        add_system_message("<span class='roommessagelabel'>" + msg + "</span>", null, index);
                    }
                    $(document).trigger("subjectchanged.defchat", subject);
                    var display_subject = settings.parse_subject(subject);
                    settings.subject.html(display_subject);
                    settings.current_subject = subject;
                };
                $.ajax({
                    url: '/tags/approved/?tags=' + checked_tags.join(','),
                    type: 'GET',
                    success: function(response) {
                        var tags = response.split(',');
                        postTitle(tags)
                    },
                    error: function() {
                        postTitle([])
                    }
                });
            },
            add_message: function(message, domele, index) {
                if (!domele) {
                    domele = settings.chatlist;
                }
                var chat = domele.get(0);
                if (window.isMobileSite) {
                    chat = $("#chat").get(0);
                }
                var should_scroll = false;
                if (!settings.embed_video_only) {
                    should_scroll = chat.scrollTop >= chat.scrollHeight - chat.clientHeight - 20;
                }
                if (index != undefined && index != 0) {
                    var prior = null;
                    var after = null;
                    domele.find(".text").each(function() {
                        if ((prior === null || $(this).attr('id') < index) && $(this).attr('id') != undefined && $(this).attr('id') > 0) {
                            prior = $(this).attr('id');
                        } else if (after == null && $(this).attr('id') != undefined && $(this).attr('id') > index) {
                            after = $(this).attr('id');
                        }
                    });
                    if (prior === null || after === null) {
                        domele.append(message);
                    } else {
                        domele.find('#' + prior).after(message);
                    }
                } else {
                    domele.append(message);
                }
                domele.find('div:last').find('a[rel*=facebox]').facebox();
                if (!window.isMobileSite) {
                    domele.find('div:last span.username').jscontext({
                        html: get_contextual_menu_html,
                        bind: 'anyclick'
                    });
                }
                if (should_scroll) {
                    chat.scrollTop = chat.scrollHeight;
                }
                while (domele.find('div.text').length > 200) {
                    domele.find('div.text:first').empty().remove();
                }
            },
            _format_message: function(message, nick, room, color, font, is_mod, has_tokens, tipped_recently, in_fanclub, background_color, index, tipped_alot_recently, tipped_tons_recently, is_pm) {
                if (nick === settings.username && session_metrics.messageSuccessCount < session_metrics.messageCount) {
                    session_metrics.messageSuccessCount += 1
                }
                if (tipped_alot_recently === undefined)
                    tipped_alot_recently = false;
                if (tipped_tons_recently === undefined)
                    tipped_tons_recently = false;
                if (index === undefined) {
                    index = 0;
                }
                if (room === settings.handler.room_owner_nick) {
                    var body = message_receiver._add_emoticons(message);
                    var mobilePrivateIndicator = '';
                    if (window.isMobileSite && is_pm) {
                        mobilePrivateIndicator = ' (private msg)';
                    }
                    var r = $('<div id="' + index + '" class="text">' + '<p style="display: inline-block; width: 100%">' + '<span class="username messagelabel" ' + 'data-nick="' + nick + '">' + nick + mobilePrivateIndicator + ':</span>' + body + '</p>' + '</div>');
                    if (font !== 'default') {
                        r.css('font-family', font);
                    }
                    if (background_color) {
                        r.find('p').css('background', background_color);
                    }
                    r.css('color', color);
                    if (nick === settings.handler.room_owner_nick) {
                        r.find('.messagelabel').addClass('hostmessagelabel');
                    } else if (is_mod) {
                        r.find('.messagelabel').addClass('moderatormessagelabel');
                    } else if (in_fanclub) {
                        r.find('.messagelabel').addClass('fanclubmessagelabel');
                    } else if (tipped_tons_recently) {
                        r.find('.messagelabel').addClass('tippedtonsrecentlymessagelabel');
                    } else if (tipped_alot_recently) {
                        r.find('.messagelabel').addClass('tippedalotrecentlymessagelabel');
                    } else if (tipped_recently) {
                        r.find('.messagelabel').addClass('tippedrecentlymessagelabel');
                    } else if (has_tokens) {
                        r.find('.messagelabel').addClass('hastokensmessagelabel');
                    }
                }
                return r;
            },
            _add_emoticons: function(in_body) {
                var body = in_body;
                while (true) {
                    var match = body.match(/%%%\[emoticon ([^\s]+)\]%%%/);
                    if (!match) {
                        break;
                    }
                    var urlparts = match[1].split(/\|/);
                    var show_icon = settings.show_emoticons;
                    var ignored_emoticons = settings.ignored_emoticons;
                    var str = '<a class="facebox_link@IS_THUMBNAIL@" href="#report_emoticon" rel="facebox"><input type="hidden" name="image_url" value="@IMAGE_URL@" /><input type="hidden" name="report_url" value="@REPORT_URL@" />@IMG_HTML@</a>';
                    var emoticonScale = 1;
                    if (window.isMobileSite) {
                        str = '@IMG_HTML@';
                        emoticonScale = .5;
                    }
                    var is_thumbnail = (urlparts.length == 6);
                    var img_html = '<img src="' + urlparts[1] + '" title=":' + urlparts[0] + '" height="' + (parseInt(urlparts[3]) * emoticonScale) + '" width="' + (parseInt(urlparts[2]) * emoticonScale) + '" style="" />';
                    if (!(show_icon) || $.inArray(urlparts[0], defchat_settings.ignored_emoticons) >= 0) {
                        img_html = img_html.replace('style=""', 'style="display: none;"') + '<span class="emoticon_slug">:' + urlparts[0] + '</span>';
                    }
                    str = str.replace('@IMG_HTML@', img_html);
                    if (is_thumbnail) {
                        str = str.replace('@IS_THUMBNAIL@', ' thumbnail');
                        str = str.replace('@IMAGE_URL@', urlparts[4]);
                    } else {
                        str = str.replace('@IS_THUMBNAIL@', '');
                        str = str.replace('@IMAGE_URL@', urlparts[1]);
                    }
                    str = str.replace('@REPORT_URL@', urlparts[urlparts.length - 1]);
                    body = body.replace(match[0], str);
                }
                return body;
            },
            _validate_hex_color: function(color, default_color) {
                if (/^#[0-9a-f]{3}([0-9a-f]{3})?$/i.test(color)) {
                    return color;
                }
                return default_color;
            },
            _validate_font_weight: function(weight, default_weight) {
                if (weight === 'normal' || weight === 'bold' || weight === 'bolder') {
                    return weight;
                }
                return default_weight;
            }
        };
        var apps_and_bots = {
            refresh_panel: function() {
                $('.info-user a[data-tab="apps_and_bots"]').click();
            }
        };
        var groups_and_privates = {
            init: function() {
                $('.start_group_show').click(groups_and_privates.start_group_show_clicked);
                $('.start_group_show_cancel').click(groups_and_privates.cancel_group_chat_request);
                $('.start_private_show').click(groups_and_privates.start_private_show_clicked);
                $('.start_private_show_cancel').click(groups_and_privates.cancel_private_chat_request);
                $('.broadcaster_panel_show_requested .accept').click(groups_and_privates.approve_private_show_clicked);
                $('.broadcaster_panel_show_requested .decline').click(groups_and_privates.decline_private_show_clicked);
                $('.broadcaster_panel_private_show .decline').click(groups_and_privates.decline_private_show_clicked);
                $('.broadcaster_panel_group_show_requested .accept').click(groups_and_privates.approve_group_show_clicked);
                $('.broadcaster_panel_group_show_requested .decline').click(groups_and_privates.decline_group_show_clicked);
                $('.broadcaster_panel_group_show .decline').click(groups_and_privates.decline_group_show_clicked);
                $('.start_group_show_link').live('click', groups_and_privates.start_group_show_clicked);
                $('.return_from_away_mode').live('click', groups_and_privates.return_from_away_mode_clicked);
                $('.spy_on_private_link').live('click', groups_and_privates.start_spy_on_private_clicked);
            },
            approve_private_show_clicked: function() {
                if (groups_and_privates.is_responding_to_private_request()) {
                    $.post(settings.private_show_approve_url, {
                        'foo': 'bar'
                    });
                }
            },
            decline_private_show_clicked: function() {
                if (!confirm(gettext("Are you sure?"))) {
                    return
                }
                $.post(settings.private_show_decline_url, {
                    'foo': 'bar'
                });
            },
            approve_group_show_clicked: function() {
                $.post(settings.group_show_approve_url, {
                    'foo': 'bar'
                });
            },
            return_from_away_mode_clicked: function() {
                $.post(settings.return_from_away_url, {
                    'foo': 'bar'
                });
            },
            decline_group_show_clicked: function() {
                if (!confirm(gettext("Are you sure?"))) {
                    return
                }
                $.post(settings.group_show_decline_url, {
                    'foo': 'bar'
                });
            },
            start_spy_on_private_clicked: function() {
                if ($('.private_show_controls').is(':visible')) {
                    return false;
                }
                if (!window.isMobileSite) {
                    if (settings.login_required_callback && settings.login_required_callback()) {
                        return false;
                    }
                } else if (window.isLoggedIn && !window.isLoggedIn()) {
                    return false;
                }
                groups_and_privates.change_private_show_link('processing');
                $.post(settings.spy_on_private_show_tokens_per_minute_url, {
                    'foo': 'bar'
                }, function(data) {
                    if (parseInt(data) == 0) {
                        var msg = gettext("Spy has been disabled by the broadcaster");
                        alert(msg);
                        return;
                    }
                    var msg = interpolate(gettext("Spy on private show for (%(price)s tokens per minute)"), {
                        modelname: settings.handler.room_owner_nick,
                        price: data
                    }, true);
                    if (confirm(msg)) {
                        settings.handler.request_spy_show().then(function(errorMsg) {
                            if (errorMsg !== "") {
                                alert(errorMsg);
                                groups_and_privates.change_private_show_link();
                                return;
                            }
                        })["catch"](groups_and_privates.error_starting_private_show);
                    } else {
                        groups_and_privates.change_private_show_link();
                    }
                }).error(groups_and_privates.error_starting_private_show);
                return true;
            },
            start_group_show_clicked: function() {
                if (!$('.group_show_controls').is(':visible') || groups_and_privates.is_requesting_group_chat()) {
                    return false;
                }
                if (settings.login_required_callback) {
                    if (settings.login_required_callback()) {
                        return false;
                    }
                }
                if (!settings.handler.groups_enabled) {
                    alert("This feature is not enabled on your device.");
                    return true;
                }
                groups_and_privates.change_group_show_link('processing');
                $.post(settings.group_show_tokens_per_minute_url, {
                    'foo': 'bar'
                }, function(data) {
                    var msg = interpolate(gettext("Enter group chat with %(modelname)s? (%(price)s tokens per minute)"), {
                        modelname: settings.handler.room_owner_nick,
                        price: data
                    }, true);
                    if (confirm(msg)) {
                        settings.handler.request_group_show(data).then(function(errorMsg) {
                            if (errorMsg !== "") {
                                alert(errorMsg);
                                groups_and_privates.change_group_show_link();
                                return
                            }
                            if (settings.handler.initializer.model_status === "group") {
                                groups_and_privates.change_group_show_link('active');
                            } else if (!$('.start_group_show_active').is(':visible')) {
                                groups_and_privates.change_group_show_link('waiting');
                            }
                        })["catch"](groups_and_privates.error_starting_group_show);
                    } else {
                        groups_and_privates.change_group_show_link();
                    }
                }).error(groups_and_privates.error_starting_group_show);
                return true;
            },
            show_group_show_waiting_message: function(users_waiting, users_required, index) {
                if (index === undefined) {
                    index = 0;
                }
                if (!settings.handler.initializer.is_on_private_room) {
                    add_system_message("<span class='privatealert'>" + parseInt(users_waiting) + " of " + parseInt(users_required) + " users are ready to start a group show (" + "<a href='#' class='start_group_show_link'>join group show</a> | " + "<a href='" + settings.spy_on_cams_url + "'>more shows</a>)</span>", null, index);
                }
            },
            on_groupshowrequest_message: function(users_waiting, users_required, tokens_per_minute, index) {
                if (settings.is_broadcasting & !settings.handler.initializer.is_on_private_room) {
                    if (parseInt(users_waiting) == parseInt(users_required)) {
                        $('.broadcaster_panel_group_show_requested .tokens_per_minute').html(tokens_per_minute);
                        groups_and_privates.change_broadcaster_panel('group_show_requested');
                    }
                }
            },
            start_private_show_clicked: function() {
                if (settings.login_required_callback) {
                    if (settings.login_required_callback()) {
                        return false;
                    }
                }
                if (!settings.handler.privates_enabled) {
                    alert("This feature is not enabled on your device.");
                    return;
                }
                groups_and_privates.change_private_show_link('processing');
                $.post(settings.private_show_tokens_per_minute_url, {
                    'foo': 'bar'
                }, function(data) {
                    try {
                        var response_data = $.parseJSON(data);
                    } catch (e) {
                        groups_and_privates.error_starting_private_show();
                        return;
                    }
                    var msg = interpolate(gettext("Enter private chat with %(modelname)s?\n%(price)s tokens per minute, " + "minimum %(private_show_minimum_minutes)s minute(s)"), {
                        modelname: settings.handler.room_owner_nick,
                        price: response_data.price,
                        private_show_minimum_minutes: response_data.private_show_minimum_minutes
                    }, true);
                    if (response_data.recordings_allowed) {
                        msg += "\n\nThis broadcaster allows private show recordings, so you will receive a recorded video of this show in your collection."
                    } else {
                        msg += "\n\nThis broadcaster does not allow private show recordings, so you will not receive a recorded video of this show in your collection."
                    }
                    if (confirm(msg)) {
                        settings.handler.request_private_show(response_data.price, response_data.private_show_minimum_minutes).then(function(errorMsg) {
                            if (errorMsg !== "") {
                                alert(errorMsg);
                                groups_and_privates.change_private_show_link();
                            } else {
                                groups_and_privates.change_private_show_link('waiting');
                            }
                        })["catch"](groups_and_privates.error_starting_private_show);
                    } else {
                        groups_and_privates.change_private_show_link();
                    }
                }).error(groups_and_privates.error_starting_private_show);
            },
            is_requesting_group_chat: function() {
                return $('.start_group_show_waiting').is(':visible') | $('.start_group_show_processing').is(':visible');
            },
            is_start_group_show_active: function() {
                return $('.start_group_show_active').is(':visible');
            },
            is_requesting_private_chat: function() {
                return $('.start_private_show_waiting').is(':visible');
            },
            is_responding_to_private_request: function() {
                return $('.broadcaster_panel_show_requested').is(':visible');
            },
            on_groupshowrequest_approvemessage: function(tokens_per_minute) {
                add_system_message("<span class='privatealert'>" +
                    gettext("Group show has started.") + "</span>");
                if (settings.handler.initializer.is_on_private_room) {
                    return;
                }
                $('.tokens_per_minute').html(tokens_per_minute);
                if (groups_and_privates.is_requesting_group_chat() || groups_and_privates.is_start_group_show_active() || settings.is_broadcasting) {
                    groups_and_privates.change_group_show_link('active');
                    groups_and_privates.change_broadcaster_panel('group_show');
                    settings.handler.initializer.change_to_private_room(true);
                } else {
                    settings.handler.set_model_status("group");
                }
            },
            on_privateshowrequest_approvemessage: function(tokens_per_minute) {
                add_system_message("<span class='privatealert'>" +
                    gettext("Private show has started.") + "</span>");
                if (settings.handler.initializer.is_on_private_room) {
                    return;
                }
                if (groups_and_privates.is_requesting_private_chat() | settings.is_broadcasting) {
                    groups_and_privates.change_private_show_link('active');
                    groups_and_privates.change_broadcaster_panel('private_show');
                    $('.tokens_per_minute').html(tokens_per_minute);
                    settings.is_private_show_owner = true;
                    settings.handler.initializer.change_to_private_room(false);
                } else {
                    settings.handler.set_model_status("private");
                }
            },
            change_broadcaster_panel: function(name) {
                if (public_methods.is_goal_set() && name == 'default') {
                    name = 'goal_display';
                }
                $('.broadcaster_panel_default').hide();
                $('.broadcaster_panel_private_show').hide();
                $('.broadcaster_panel_group_show').hide();
                $('.broadcaster_panel_show_requested').hide();
                $('.broadcaster_panel_group_show_requested').hide();
                $('.broadcaster_panel_goal_configuration').hide();
                $('.broadcaster_panel_goal_display').hide();
                $('.broadcaster_panel_away').hide();
                $(".broadcaster_panel_" + name).show();
            },
            on_groupshowrequest_cancelmessage: function(index) {
                if (index === undefined) {
                    index = 0;
                }
                $('.num_users_waiting_for_group').html('0');
                if (settings.handler.initializer.is_on_private_room) {
                    if (settings.is_broadcasting) {
                        groups_and_privates.change_broadcaster_panel('away');
                    }
                    add_system_message("<span class='privatealert'>" +
                        gettext("Group show has finished.") + "</span>", null, index);
                    settings.handler.initializer.leave_private_room();
                    groups_and_privates.change_group_show_link();
                } else {
                    if (groups_and_privates.is_requesting_group_chat()) {
                        add_system_message("<span class='privatealert'>" +
                            gettext("Group show has been declined.") + "</span>", null, index);
                        groups_and_privates.change_group_show_link();
                    }
                    if (settings.is_broadcasting) {
                        groups_and_privates.change_broadcaster_panel('default');
                    }
                }
            },
            on_awaystatus_cancelmessage: function() {
                if (settings.is_broadcasting) {
                    groups_and_privates.change_broadcaster_panel('default');
                }
            },
            on_privateshowrequest_cancelmessage: function(index) {
                if (index === undefined) {
                    index = 0;
                }
                if (settings.handler.initializer.is_on_private_room) {
                    add_system_message("<span class='privatealert'>" +
                        gettext("Private show has finished.") + "</span>");
                    settings.handler.initializer.leave_private_room();
                    groups_and_privates.change_private_show_link();
                    if (settings.is_broadcasting) {
                        groups_and_privates.change_broadcaster_panel('away');
                    }
                } else {
                    if (groups_and_privates.is_requesting_private_chat()) {
                        add_system_message("<span class='privatealert'>" +
                            gettext("Private show has been declined.") + "</span>");
                        groups_and_privates.change_private_show_link();
                    } else if (!settings.is_broadcasting && settings.handler.initializer.model_status == 'private') {
                        settings.handler.set_model_status('away');
                    }
                    if (settings.is_broadcasting) {
                        groups_and_privates.change_broadcaster_panel('default');
                    } else {
                        groups_and_privates.change_private_show_link();
                        add_system_message("<span class='privatealert'>" +
                            gettext("Private show has finished.") + "</span>", null, index);
                    }
                }
            },
            on_leaveprivateroom_message: function(username) {
                if (username == settings.my_username) {
                    settings.handler.initializer.leave_private_room();
                    groups_and_privates.change_group_show_link();
                }
            },
            on_privateshowrequest_message: function(requester_username, tokens_per_minute) {
                if (settings.is_broadcasting) {
                    $('.broadcaster_panel_show_requested .requester_username').html(requester_username);
                    $('.broadcaster_panel_show_requested .tokens_per_minute').html(tokens_per_minute);
                    groups_and_privates.change_broadcaster_panel('show_requested');
                    $('.privatealert').unbind('click', groups_and_privates.approve_private_show_clicked);
                    add_system_message("<span class='privatealert' style='padding: 0'>" +
                        requester_username + " wants to start a private show (<a href=\"#\">start private show</a>)</span>");
                    $('.privatealert').bind('click', groups_and_privates.approve_private_show_clicked);
                }
            },
            error_starting_group_show: function() {
                alert(gettext("There was an error requesting your show. Please try again."));
                groups_and_privates.change_group_show_link();
            },
            error_starting_private_show: function() {
                alert(gettext("There was an error requesting your show. Please try again."));
                groups_and_privates.change_private_show_link();
            },
            cancel_group_chat_request: function() {
                if (confirm(gettext("Are you sure?"))) {
                    settings.handler.leave_group_show();
                    groups_and_privates.change_group_show_link();
                }
            },
            cancel_private_chat_request: function() {
                if (confirm(gettext("Are you sure?"))) {
                    settings.handler.leave_private_show().then(function(errorMsg) {
                        if (errorMsg !== "") {
                            alert(errorMsg);
                            return;
                        }
                    })
                }
            },
            change_group_show_link: function(link) {
                $('.start_group_show').hide();
                $('.start_group_show_waiting').hide();
                $('.start_group_show_processing').hide();
                $('.start_group_show_active').hide();
                var key = '.start_group_show';
                if (link) {
                    key = key + "_" + link;
                    groups_and_privates.hide_private_show_controls();
                } else {
                    groups_and_privates.show_private_show_controls();
                }
                $(key).show();
            },
            change_private_show_link: function(link) {
                $('.start_private_show').hide();
                $('.start_private_show_processing').hide();
                $('.start_private_show_waiting').hide();
                $('.start_private_show_active').hide();
                var key = '.start_private_show';
                if (link) {
                    key = key + "_" + link;
                    groups_and_privates.hide_group_show_controls();
                } else {
                    groups_and_privates.show_group_show_controls();
                }
                $(key).show();
            },
            hide_private_show_controls: function(force) {
                $('.private_show_controls').hide();
                if (force) {
                    $('.private_show_controls').data('forcehidden', '1');
                }
                groups_and_privates.show_group_show_controls();
            },
            show_private_show_controls: function(force) {
                if (force | $('.private_show_controls').data('forcehidden') != '1') {
                    $('.private_show_controls').show();
                }
            },
            hide_group_show_controls: function(force) {
                $('.group_show_controls').hide();
                if (force) {
                    $('.group_show_controls').data('forcehidden', '1');
                }
            },
            show_group_show_controls: function(force) {
                if (force | $('.group_show_controls').data('forcehidden') != '1') {
                    $('.group_show_controls').show();
                }
            }
        };
        groups_and_privates.init();
        var tipping = {
            current_balance: 0,
            tipped_performer_last_24hrs: 0,
            last_tip_option: '',
            init: function() {
                $(".tip_button").click(tipping.on_tipping_button_clicked);
                settings.domroot.find('.tip_popup form').submit(tipping.on_form_submit);
                tipping.current_balance = settings.token_balance;
                tipping.tipped_performer_last_24hrs = settings.tipped_performer_last_24hrs;
                $(document).bind('keydown', 'ctrl+s', tipping.on_key_down);
                $(document).bind('keydown', 'command+s', tipping.on_key_down);
                $(document).bind('keydown', 'esc', tipping._hide_tip_overlay);
                $("#rate_up").click(function() {
                    tipping.rate_model(10);
                });
                $("#rate_down").click(function() {
                    tipping.rate_model(0);
                });
                tipping.check_rating_ctrl();
            },
            _hide_tip_overlay: function(event) {
                var ep = settings.domroot.find(".tip_popup");
                if (ep.is(":visible")) {
                    $("body").unbind('click', tipping.on_body_clicked_while_overlay_up);
                    $("#id_tip_amount").blur();
                    $("#tip_message").blur();
                    ep.hide();
                    public_methods.enable_html5_video_controls();
                    return true;
                }
                return false;
            },
            on_key_down: function(event) {
                tipping.toggle_overlay();
                return false;
            },
            on_form_submit: function() {
                var amount = parseInt($("#id_tip_amount").val());
                if (isNaN(amount) | amount < 1) {
                    session_metrics.tipFailCount += 1;
                    alert(gettext("Invalid tip amount."));
                    return false;
                }
                if (amount > tipping.current_balance) {
                    session_metrics.tipFailCount += 1;
                    session_metrics.tipFailTotal += amount;
                    alert(gettext("You do not have enough tokens."));
                    return false;
                }
                var msg = interpolate(gettext("Send %(tokens)s tokens to %(username)s?"), {
                    username: settings.performer,
                    tokens: amount
                }, true);
                tipping.last_tip_option = $("#tip_options_select").val();
                if (amount > 100) {
                    if (!confirm(msg)) {
                        return false;
                    }
                }
                var $form = $(this);
                $.ajax({
                    url: $(this).attr('action'),
                    dataType: 'json',
                    data: $(this).serialize(),
                    type: 'POST',
                    success: function(response) {
                        if (response.error) {
                            session_metrics.tipFailCount += 1;
                            session_metrics.tipFailTotal += amount;
                            alert(response.error);
                        } else if (response.success) {
                            session_metrics.tipSuccessCount += 1;
                            session_metrics.tipSuccessTotal += amount;
                            $form.find("textarea").val("");
                            tipping.current_balance = parseInt(response.token_balance);
                            tipping.tipped_performer_last_24hrs = parseInt(response.tipped_performer_last_24hrs);
                            $('.tokencount').html(response.token_balance);
                            $('.balance ').load(window.location.pathname + ' .balance');
                            $('.token_balance').load(window.location.pathname + ' .token_balance');
                            tipping.check_rating_ctrl();
                        } else {
                            session_metrics.tipFailCount += 1;
                            session_metrics.tipFailTotal += amount;
                            alert('An error occurred.');
                        }
                    }
                });
                tipping.toggle_overlay();
                return false;
            },
            rate_model: function(rating) {
                if (rating === settings.last_vote_in_past_24_hours || (rating < 5 && !confirm(interpolate(gettext("Are you sure you want to rate %(username)s thumbs down?"), {
                        username: settings.performer.ucfirst()
                    }, true)))) {
                    return false;
                }
                $.ajax({
                    url: settings.rate_model_url,
                    dataType: 'json',
                    data: {
                        csrfmiddlewaretoken: $.cookie('csrftoken'),
                        rating: rating
                    },
                    type: 'POST',
                    success: function(response) {
                        if (response.error) {
                            alert(response.error);
                        } else if (response.success) {
                            settings.last_vote_in_past_24_hours = rating;
                            tipping.highlight_chosen_thumb();
                            tipping.rating_response_message(interpolate(gettext("Note: Your confidential vote of %(vote)s for %(performer)s has been recorded. You may change your vote at any time today. Thank you for your feedback."), {
                                vote: rating >= 5 ? 'thumbs up' : 'thumbs down',
                                performer: settings.performer.ucfirst()
                            }, true));
                        } else {
                            tipping.rating_response_message(interpolate(gettext("Note: Your vote for %(performer)s was NOT recorded. Your 24 hour window of eligibility to vote may have very recently expired."), {
                                performer: settings.performer.ucfirst()
                            }, true));
                            tipping.tipped_performer_last_24hrs = 0;
                            tipping.check_rating_ctrl();
                        }
                    }
                });
            },
            highlight_chosen_thumb: function() {
                if (settings.last_vote_in_past_24_hours === undefined) {
                    return false;
                }
                if (settings.last_vote_in_past_24_hours >= 5) {
                    $("#thumb_up_off").hide();
                    $("#thumb_up_on").show();
                    $("#thumb_down_on").hide();
                    $("#thumb_down_off").show();
                } else {
                    $("#thumb_up_on").hide();
                    $("#thumb_up_off").show();
                    $("#thumb_down_off").hide();
                    $("#thumb_down_on").show();
                }
                return true;
            },
            rating_response_message: function(msg) {
                if ($('.chat-box').length) {
                    add_system_message(msg);
                } else {
                    setTimeout(function() {
                        alert(msg);
                    }, 100);
                }
            },
            check_rating_ctrl: function() {
                var eligible = (tipping.tipped_performer_last_24hrs >= settings.min_tips_reqd_to_vote);
                if (tipping.eligible_to_vote === false && eligible) {
                    setTimeout(function() {
                        tipping.rating_response_message(interpolate(gettext("Note: For tipping at least %(min)s tokens today, you can now vote what you think about %(performer)s. Your vote is confidential. See below for the satisfaction feedback controls."), {
                            min: settings.min_tips_reqd_to_vote,
                            performer: settings.performer.ucfirst()
                        }, true));
                    }, 500);
                }
                if (eligible) {
                    $("#model_rating_ctrl_dim").hide();
                    $("#model_rating_ctrl").show();
                    $("#model_rating_ctrl_dim_with_scores").hide();
                    $("#model_rating_ctrl_with_scores").show();
                    tipping.highlight_chosen_thumb();
                } else {
                    $("#model_rating_ctrl").hide();
                    $("#model_rating_ctrl_dim").show();
                    $("#model_rating_ctrl_with_scores").hide();
                    $("#model_rating_ctrl_dim_with_scores").show();
                }
                tipping.eligible_to_vote = eligible;
                return eligible
            },
            on_tipping_button_clicked: function(ev) {
                if (settings.login_required_callback) {
                    if (settings.login_required_callback()) {
                        return false;
                    }
                }
                if (!(tabs.current_tab == 'pm' | tabs.current_tab == 'chat')) {
                    tabs._show_chat_tab();
                }
                tipping.toggle_overlay();
            },
            on_body_clicked_while_overlay_up: function(ev) {
                if (settings.domroot.find('.tip_popup').has(ev.target).length || $('.tip_button').is(ev.target)) {
                    return;
                }
                tipping.toggle_overlay();
            },
            toggle_overlay: function(amount, message) {
                if (!settings.allow_tipping) {
                    return;
                }
                if (!(typeof _gaq === 'undefined')) {
                    _gaq.push(['_trackPageview', '/tracking/toggle_tipping_overlay/']);
                }
                var ep = settings.domroot.find(".tip_popup");
                if (!tipping._hide_tip_overlay()) {
                    $("body").click(tipping.on_body_clicked_while_overlay_up);
                    public_methods.disable_html5_video_controls();
                    $.ajax({
                        url: settings.tipping_current_tokens_url,
                        type: 'GET',
                        data: 'foo=bar&room=' + settings.handler.room_owner_nick,
                        dataType: 'json',
                        cache: false,
                        success: function(response) {
                            tipping.current_balance = parseInt(response.token_balance);
                            $("#tip_message").show();
                            $("#tip_options").hide();
                            $("#tip_options_select").empty();
                            if (response.tip_options) {
                                try {
                                    var tipoptions = $.parseJSON(response.tip_options).tip_options;
                                    if (tipoptions) {
                                        $("#tip_message").hide();
                                        $("#tip_options").show();
                                        $("#tip_options_select").empty();
                                        $("#tip_options_select_label").text(tipoptions.label);
                                        $("#tip_options_select").append($("<option/>").attr("value", "").text("-- Select One --"));
                                        $(tipoptions.options).each(function(index, elem) {
                                            var newoption = $("<option/>").attr("value", elem.label).text(elem.label);
                                            if (elem.label == tipping.last_tip_option) {
                                                newoption.attr("selected", '1');
                                            }
                                            $("#tip_options_select").append(newoption);
                                        });
                                    }
                                } catch (err) {
                                    alert("Unable to construct tip panel. Perhaps an app returned invalid tip options.")
                                    return;
                                }
                            }
                            $('.tokencount').html(response.token_balance);
                            ep.css('z-index', '999999');
                            ep.show();
                            ep.find('#id_tip_amount').focus();
                            ep.find('#id_tip_msg_input').val("");
                            if (amount !== 'undefined' && typeof amount === "number" && !isNaN(amount)) {
                                ep.find('#id_tip_amount').val(amount);
                                if (message !== 'undefined' && typeof message === "string" && message.length > 0) {
                                    ep.find('#id_tip_msg_input').val(message);
                                }
                            }
                            ep.find('#id_tip_amount').select();
                        }
                    });
                }
            }
        };
        tipping.init();

        function shiftCode(shift, code) {
            if (code === 27 || code === 8 || code === 9 || code === 20 || code === 16 || code === 17 || code === 91 || code === 13 || code === 92 || code === 18) {
                return '';
            }
            if (typeof shift != "boolean" || typeof code != "number") {
                return '';
            }
            var shift_chars = [];
            shift_chars[192] = "~";
            shift_chars[49] = "!";
            shift_chars[50] = "@";
            shift_chars[51] = "#";
            shift_chars[52] = "$";
            shift_chars[53] = "%";
            shift_chars[54] = "^";
            shift_chars[55] = "&";
            shift_chars[56] = "*";
            shift_chars[57] = "(";
            shift_chars[48] = ")";
            shift_chars[109] = "_";
            shift_chars[189] = "_";
            shift_chars[107] = "+";
            shift_chars[187] = "+";
            shift_chars[219] = "{";
            shift_chars[221] = "}";
            shift_chars[220] = "|";
            shift_chars[59] = ":";
            shift_chars[186] = ":";
            shift_chars[222] = "\"";
            shift_chars[188] = "<";
            shift_chars[190] = ">";
            shift_chars[191] = "?";
            shift_chars[32] = " ";
            var special_chars = [];
            special_chars[192] = '`';
            special_chars[189] = '-';
            special_chars[187] = '=';
            special_chars[219] = '[';
            special_chars[221] = ']';
            special_chars[220] = '\\';
            special_chars[186] = ';';
            special_chars[222] = '\'';
            special_chars[188] = ',';
            special_chars[190] = '.';
            special_chars[191] = '/';
            if (shift) {
                if (code >= 65 && code <= 90) {
                    return String.fromCharCode(code);
                }
                return shift_chars[code];
            } else {
                if (code >= 65 && code <= 90) {
                    return String.fromCharCode(code).toLowerCase();
                }
                if (special_chars[code]) {
                    return special_chars[code];
                }
                return String.fromCharCode(code);
            }
        }
        var keybinder = {
            current_focus: null,
            init: function() {
                $(document).keydown(keybinder.handle_keydown);
                $(":input").live('focus', keybinder.on_focus);
                $(":input").live('blur', keybinder.on_blur);
            },
            handle_keydown: function(e) {
                if (window.isMobileSite) {
                    return;
                }
                if (e.which === 8 && !$(e.target).is("input, textarea")) {
                    settings.textinput.focus();
                    return;
                }
                if (keybinder.current_focus !== null || e.metaKey) {
                    return;
                }
                if (e.keyCode === 13) {
                    message_sender.on_form_submit();
                    return;
                }
                if ((e.keyCode >= 65 && e.keyCode <= 90) || (e.keyCode >= 48 && e.keyCode <= 57) || (e.keyCode >= 186 && e.keyCode <= 222)) {
                    if ($('#xmovie').is(":focus")) {
                        settings.textinput.focus().val(settings.textinput.val() +
                            shiftCode(e.shiftKey, e.keyCode));
                    } else {
                        settings.textinput.focus().val(settings.textinput.val());
                    }
                }
            },
            on_blur: function() {
                keybinder.current_focus = null;
            },
            on_focus: function() {
                keybinder.current_focus = this.id;
            }
        };
        keybinder.init();
        tabs.apply_font_size();
        settings.form.submit(message_sender.on_form_submit);
        settings.submitbutton.click(message_sender.on_form_submit);
        if (settings.throttlechat) {
            if (settings.welcome_message) {
                add_system_message(settings.welcome_message);
            }
            if (settings.welcome_warning) {
                add_system_message(settings.welcome_warning);
            }
            add_system_message(settings.throttle_message);
        } else {
            if (!settings.do_not_connect) {
                settings.handler.connect(message_receiver, settings.room, settings, groups_and_privates);
            }
        }
        if (settings.is_broadcasting) {
            message_receiver.on_subject_change(settings.handler.sanitize_room_subject(settings.default_subject), false);
        }
        if (settings.initial_viewer_count !== null) {
            public_methods.update_viewer_count(settings.initial_viewer_count);
        }

        function zoomDisable() {
            $('head meta[name=viewport]').remove();
            $('head').prepend('<meta name="viewport" content="user-scalable=0" />');
        }

        function zoomEnable() {
            $('head meta[name=viewport]').remove();
            $('head').prepend('<meta name="viewport" content="user-scalable=1" />');
        }

        function allow_scroll_saving() {
            settings.scroll_saving_allowed = true;
        }

        function save_scrolling() {
            settings.scroll_saving_allowed = false;
            setTimeout(allow_scroll_saving, 1000);
            settings.saved_scroll_top = $(document).scrollTop();
            zoomDisable();
        }

        function load_scrolling() {
            $(document).scrollTop(settings.saved_scroll_top);
            zoomEnable();
        }
        settings.textinput.focus(save_scrolling);
        settings.textinput.focusout(load_scrolling);
        $('#id_tip_msg_input').focus(save_scrolling);
        $('#id_tip_msg_input').focusout(load_scrolling);
        $("#id_tip_amount").focus(save_scrolling);
        $("#id_tip_amount").focusout(load_scrolling);
        $('#id_additional_comments').focus(save_scrolling);
        $('#id_additional_comments').focusout(load_scrolling);
        $(document).scroll(function() {
            if (settings.scroll_saving_allowed && settings.textinput.is(":focus")) {
                settings.saved_scroll_top = $(document).scrollTop();
            }
        });
        $(document).bind('keydown', 'tab', function(e) {
            if ($('#login_inputs').is(":visible")) {
                return;
            }
            e.preventDefault();
        });
        $(document).bind('keydown', 'shift+tab', function(e) {
            if ($('#login_inputs').is(":visible")) {
                return;
            }
            e.preventDefault();
        });
        $(document).bind('keyup', 'shift+tab', function(e) {
            if ($('#login_inputs').is(":visible")) {
                var focus_within = false;
                $('#login_inputs').children().each(function() {
                    $(this).children().each(function() {
                        if ($(this).is(":focus")) {
                            focus_within = true;
                        }
                    });
                });
                if (!focus_within) {
                    $('#id_forgotpw_link').focus();
                }
            }
        });
        $(document).bind('keyup', 'tab', function(e) {
            if ($('#login_inputs').is(":visible")) {
                var focus_within = false;
                $('#login_inputs').children().each(function() {
                    $(this).children().each(function() {
                        if ($(this).is(":focus")) {
                            focus_within = true;
                        }
                    });
                });
                if (!focus_within) {
                    $('#id_close_link').focus();
                }
            }
        });
        apply_input_tab_override($('#id_tip_amount, #id_tip_msg_input, #id_tip_message'));
        apply_input_tab_override($('#id_tip_amount, #tip_options_select, #id_tip_message'));
        apply_input_tab_override($('#id_abuse_category_select, #id_additional_comments, ' + '#abuse_cancel_button, #abuse_report_button'));
        var returnhandle = function(method) {
            if (public_methods[method]) {
                return public_methods[method].apply(this, Array.prototype.slice.call(arguments, 1));
            } else if (typeof method === 'object' || !method) {
                return public_methods.init.apply(this, arguments);
            } else {
                $.error('Method ' + method + ' does not exist in defchat');
            }
        };
        return returnhandle;
    };
})(jQuery);
var notice_timeout;

function display_notice(message) {
    var notice = $('#settings_notice');
    notice.stop(true, true);
    clearTimeout(notice_timeout);
    var chat_box = $('.chat-box');
    notice.text(message);
    notice.css({
        top: chat_box.offset().top + 30,
        left: chat_box.offset().left + chat_box.width() - notice.width() - 32
    });
    notice.fadeIn('fast');
    notice_timeout = setTimeout(function() {
        notice.fadeOut('slow');
    }, 3000);
}

function isAnyIE() {
    if (navigator.appName == 'Microsoft Internet Explorer' || !!(navigator.userAgent.match(/Trident/) || navigator.userAgent.match(/rv 11/)) || $.browser.msie == 1)
        return true;
    else
        return false;
}