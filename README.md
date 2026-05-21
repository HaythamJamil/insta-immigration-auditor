# insta-immigration-auditor

An open source tool that scans your Instagram export and flags content that may be scrutinized during US immigration vetting. Helps everyday people audit and clean their own social media history before it gets used against them. All processing is local. Your data never leaves your machine.

\# Instagram Immigration Audit Tool



A free, open-source tool for individuals preparing for immigration 

processes to audit their own Instagram activity for content that 

may be flagged during social media vetting by agencies like USCIS/DHS.



\## What this tool does

\- Scans your personal Instagram data export (JSON) locally on your machine

\- Compares your likes, comments, and follows against a community-maintained 

&#x20; keyword list

\- Generates a report of potentially flagged content for your personal review

\- Includes a rate-limited cleanup script to remove flagged content via 

&#x20; Instagram's API



\## What this tool does NOT do

\- It never uploads your data anywhere — everything runs locally

\- It does not make final decisions — it surfaces candidates for YOUR review

\- It is not legal advice



\## Who is this for

Anyone going through a US immigration process who wants to proactively 

audit their own social media history.



\## Contributing

The keyword list is the most important part of this project and needs 

community input. If you can contribute slang, transliterations, or 

keywords in any language please open a pull request against the 

relevant file in /keywords.



\## Disclaimer

This tool is for personal defensive use only.



\## How it works



\### Step 1 — Export your Instagram data

Request your data export from Instagram Settings → Your Activity 

→ Download Your Information. Select JSON format.



\### Step 2 — Run the scanner (C++)

Scans your export locally and generates flagged\_report.json



\### Step 3 — Review the report

Go through the flagged items manually and decide what to remove



\### Step 4 — Run the cleaner (Python/instagrapi)

Automatically removes flagged content at 100-150 actions/hour 

with randomized delays to avoid triggering Instagram's bot detection

