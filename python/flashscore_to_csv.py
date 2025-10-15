# Matches from www.flashscore.com/ are copy-pasted into one file at
# ../temp/LEAGUE_NAME.raw and this script translates it into
# a LEAGUE_NAME.csv file at the same location. The temp folder is not gitted.
#
# There are deviations from consistency. Apart from the usual league games,
# there are relegations play-offs etc. which need to be filtered
# out when parsing the raw file. The teams' names are duplicated for
# some reason either in seperate rows:
# TEAM_NAME
# TEAM_NAME
# or concatenated in a single line
# TEAM_NAMETEAM_NAME
# The results are concatenated as well, so a 2-1 score looks like this
# 21
# it's easy to split but if the result is 3-4 chars long, that means
# double-digits and the results must be inputted manually. It's not impractical
# because it's rare.
#
# Some single-character artefacts sometimes appear which I think might
# refer to multiple red cards so that's just going to be ignored.
#
# Each season starts with the season name in format "YYY1-YYY2" eg.
# 2025-2026.
#
# The time and date is given in format "DD.MM. HH:MM", so the year will have
# to be inferred. I will probably discard the time because some leagues
# in the database don't have them anyway and I probably won't use them.
#
# This script is only useful until flashscore changes anything about
# their formatting.

leagues = [
    "eredivisie",               # NL
    "ekstraklasa",              # PL
    "jupiler_pro_league",       # BE
    "primeira_liga",            # PO
    "prva_hrvatska",            # HR
    "swiss_super_league",       # CH
    "denmark_superliga",        # DK
    "turkey_super_lig",         # TR
    "ukraine_premier_league",   # UK
    "brasileiro_serie_a",       # BR
    ]

countries = [
    "NETHERLANDS:",
    "POLAND:",
    "BELGIUM:",
    "PORTUGAL:",
    "CROATIA:",
    "SWITZERLAND:",
    "DENMARK:",
    "TURKEY:",
    "UKRAINE:",
    "BRAZIL:",
    ]

main_competitions = [
    "Eredivisie",
    "Ekstraklasa",
    "Jupiler Pro League", "Jupiler League",
    "Liga Portugal", "Primeira Liga",
    "HNL", "1. HNL",
    "Super League",
    "Superliga",
    "Super Lig",
    "Premier League", "Pari-Match League",
    "Serie A Betano", "Serie A", "Série A",
    ]

ignore_lines = [
    "Standings",
    ]

ignore_lines_starting_with = [
    "Round",
    ]

for league in leagues:
    pass
