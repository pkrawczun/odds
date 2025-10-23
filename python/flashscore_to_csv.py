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

# 1. Ignore Empty lines and lines starting with 'Round' and 'Standings'
# 2. Read season in format YYYY-YYYY.
# 3. Read country in format COUNTRY:
# 4. Read league name.
# 5. If not a main competition, continue until a main competition found.
# 6. Read date and infer year.
# 7. Read teams' names.
# 8. Read results.

import os # os.path.join

leagues = [
    "eredivisie",               # NL
    "ekstraklasa",              # PL
    "jupiler_pro_league",       # BE
    "primeira_liga",            # PO
    "prva_hrvatska",            # HR
    "swiss_super_league",       # CH
    "denmark_superliga",        # DK
    "turkey_super_lig",         # TR
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
    "Serie A Betano", "Serie A", "Série A",
    ]

ignore_lines = [
    "Standings",
    "Awrd",     # walkover
    ]

ignore_lines_starting_with = [
    "Round",
    ]

def isSeason(str):
    # does str encode a season? YYYY or YYYY-YYYY
    if (
        (
            len(str) == 4 and
            str.isdigit()
            ) or
        (
            len(str) == 9 and
            str[4] == '-' and
            str[:4].isdigit() and
            str[5:].isdigit()
            )
        ):
        return True
    return False

# Data from a .raw file cannot be extracted correctly.
class RawFileSyntaxError(Exception):
    pass

teams_from_all_leagues = {}
for league in leagues:
    file_path = os.path.join("..", "temp", f"{league}.raw")
    try:
        with open(file_path, "r") as file: # file is an object
            lines = list(file) # and i need indexing so make a list from it
            #for line in lines:
            #    print(line.strip())
    except FileNotFoundError:
        print("File not found: ", file_path)
    N = len(lines)
    print("N of lines in ", file_path, ": ", N)
    tmp = []
    for i in range(len(lines)):
        line = lines[i].strip()
        # Skip, empty/white lines, lines starting with 'Standings' or 'Round', lines up to 1 char long (after stripping), skip also if this line the same as the previous one
        if len(line) <= 1:
            continue
        if line.startswith(tuple(ignore_lines + ignore_lines_starting_with)):
            continue
        if tmp and tmp[-1] == line:
            continue
        # Skip if the line is concatenated with itself
        if len(line)%2 == 0 and len(line) > 4:
            half_len = len(line) // 2
            str1 = line[:half_len]
            str2 = line[half_len:]
            if (
                str1 and
                str2 and
                (
                    str1[0].isupper() or
                    str1[0].isdigit()
                    ) and
                str1 == str2
                ):
                line = str1
        tmp.append(line)
    lines = tmp

    # SECOND PARSE. SELECT MAIN COMPETITIONS
    tmp = []
    main_competition = True
    for i in range(len(lines)):
        try:
            if (
                lines[i] in countries and
                lines[i+1] in main_competitions
                ):
                main_competition = True
            if (
                lines[i] in countries and
                lines[i+1] not in main_competitions
                ):
                main_competition = False
        except IndexError:
            pass
        if main_competition:
            tmp.append(lines[i])
    lines = tmp

    # THIRD PARSE. REMOVE {country}\n{main_competition}
    tmp = []
    for line in lines:
        if (
            line in countries or
            line in main_competitions
            ):
            continue
        tmp.append(line)
    lines = tmp

    # FOURTH PARSE. EXTRACT DATA
    teams = set() # all the league's teams will go here, unique and unsorted
    i = 0
    try :
        while i < len(lines):
            if isSeason(lines[i]):
                season = lines[i]
                if len(season) == 4 and season.isdigit():
                    start_year = season
                    end_year = season
                else:
                    start_year = season[:4]
                    end_year = season[5:]
                i += 1
                continue
            # it's not a line containing a season so it's a game
            if len(lines[i]) == 12 and lines[i][9] == ':':
                #date and time
                day = lines[i][:2]
                month = lines[i][3:5]
                #hour =
                #minutes =
            else:
                raise RawFileSyntaxError(lines[i], ": this should be in date and time format MM.DD. HH:MM")
            team1 = lines[i+1]
            team2 = lines[i+2]
            teams.add(team1)
            teams.add(team2)
            result = lines[i+3]
            if not result.isdigit():
                raise RawFileSyntaxError(result, ": wrong result")
            if result.isdigit() and len(result) == 2:
                result1 = result[0]
                result2 = result[1]
            else:
                result1 = '99' # input the results manually if either is double-digit
                result2 = '99'
            if int(month) <= 6: # do not use the 'year' variable, get the year later
                year = end_year
            else:
                year = start_year
            #print("Season, Date: ", season, ' ', year, '/', month,'/', day)
            #print (team1, ', ', team2, ', ', result1, '-', result2)
            i += 4
            # print it out like this, copy-paste into a spreadsheet and get the year from that
            print(
                '"', season, '"', ";",
                start_year, ";",
                end_year, ";",
                month, ";",
                day, ";",
                '"', team1, '"', ";",
                '"', team2, '"', ";",
                result1, ";",
                result2,
                sep = ""
                )
    except IndexError:
        pass
    teams_from_all_leagues[league] = teams
    
#    for i in lines:
#        print(i)
