--
-- PostgreSQL database dump
--

-- Dumped from database version 9.3.16
-- Dumped by pg_dump version 9.3.16
-- Started on 2017-02-26 17:05:01 MSK

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

--
-- TOC entry 1 (class 3079 OID 11791)
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- TOC entry 2071 (class 0 OID 0)
-- Dependencies: 1
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = public, pg_catalog;

--
-- TOC entry 198 (class 1255 OID 16524)
-- Name: trigger_new_rule(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION trigger_new_rule() RETURNS trigger
    LANGUAGE plpgsql
    AS $$ 
BEGIN 
delete from rules where rules.svt_id=OLD.svt_id;
insert into rules (date,svt_id) values (now(),OLD.svt_id);
return NEW;
END; 
$$;


ALTER FUNCTION public.trigger_new_rule() OWNER TO postgres;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- TOC entry 171 (class 1259 OID 16385)
-- Name: device; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE device (
    id integer NOT NULL,
    "VID" character(4),
    "PID" character(4),
    serial character varying(50),
    manufacturer character varying(50),
    product character varying(50),
    title character varying(250),
    comment character varying(250),
    person_id integer,
    reg_num character varying(32) NOT NULL
);


ALTER TABLE public.device OWNER TO postgres;

--
-- TOC entry 172 (class 1259 OID 16388)
-- Name: device_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE device_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.device_id_seq OWNER TO postgres;

--
-- TOC entry 2072 (class 0 OID 0)
-- Dependencies: 172
-- Name: device_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE device_id_seq OWNED BY device.id;


--
-- TOC entry 181 (class 1259 OID 16494)
-- Name: device_to_svt; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE device_to_svt (
    device_id integer NOT NULL,
    svt_id integer NOT NULL
);


ALTER TABLE public.device_to_svt OWNER TO postgres;

--
-- TOC entry 177 (class 1259 OID 16424)
-- Name: division; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE division (
    id integer NOT NULL,
    title character varying(50),
    comment character varying(250)
);


ALTER TABLE public.division OWNER TO postgres;

--
-- TOC entry 178 (class 1259 OID 16427)
-- Name: division_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE division_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.division_id_seq OWNER TO postgres;

--
-- TOC entry 2073 (class 0 OID 0)
-- Dependencies: 178
-- Name: division_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE division_id_seq OWNED BY division.id;


--
-- TOC entry 173 (class 1259 OID 16399)
-- Name: person; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE person (
    id integer NOT NULL,
    fam character varying(50),
    name character varying(50),
    otch character varying(50),
    comment character varying(250)
);


ALTER TABLE public.person OWNER TO postgres;

--
-- TOC entry 174 (class 1259 OID 16402)
-- Name: person_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE person_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.person_id_seq OWNER TO postgres;

--
-- TOC entry 2074 (class 0 OID 0)
-- Dependencies: 174
-- Name: person_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE person_id_seq OWNED BY person.id;


--
-- TOC entry 179 (class 1259 OID 16433)
-- Name: person_to_division; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE person_to_division (
    person_id integer NOT NULL,
    division_id integer NOT NULL
);


ALTER TABLE public.person_to_division OWNER TO postgres;

--
-- TOC entry 185 (class 1259 OID 16529)
-- Name: report; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE report (
    id integer NOT NULL,
    date timestamp without time zone,
    host inet,
    "PID" character varying(4),
    "VID" character varying(4),
    serial character varying(48),
    comment character varying(200),
    manufacturer character varying(50),
    product character varying(50)
);


ALTER TABLE public.report OWNER TO postgres;

--
-- TOC entry 184 (class 1259 OID 16527)
-- Name: report_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE report_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.report_id_seq OWNER TO postgres;

--
-- TOC entry 2076 (class 0 OID 0)
-- Dependencies: 184
-- Name: report_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE report_id_seq OWNED BY report.id;


--
-- TOC entry 182 (class 1259 OID 16509)
-- Name: rules; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE rules (
    version integer NOT NULL,
    date timestamp without time zone,
    svt_id integer
);


ALTER TABLE public.rules OWNER TO postgres;

--
-- TOC entry 183 (class 1259 OID 16512)
-- Name: rules_version_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE rules_version_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.rules_version_seq OWNER TO postgres;

--
-- TOC entry 2078 (class 0 OID 0)
-- Dependencies: 183
-- Name: rules_version_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE rules_version_seq OWNED BY rules.version;


--
-- TOC entry 175 (class 1259 OID 16410)
-- Name: svt; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE svt (
    id integer NOT NULL,
    title character varying(50),
    address inet,
    "user" character varying(250),
    password character varying(250),
    comment character varying(250),
    person_id integer,
    deploy_time timestamp without time zone,
    rule_version integer
);


ALTER TABLE public.svt OWNER TO postgres;

--
-- TOC entry 2079 (class 0 OID 0)
-- Dependencies: 175
-- Name: TABLE svt; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE svt IS 'Средства вычислительной техники';


--
-- TOC entry 176 (class 1259 OID 16413)
-- Name: svt_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE svt_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.svt_id_seq OWNER TO postgres;

--
-- TOC entry 2080 (class 0 OID 0)
-- Dependencies: 176
-- Name: svt_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE svt_id_seq OWNED BY svt.id;


--
-- TOC entry 180 (class 1259 OID 16452)
-- Name: svt_to_division; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE svt_to_division (
    svt_id integer NOT NULL,
    division_id integer NOT NULL
);


ALTER TABLE public.svt_to_division OWNER TO postgres;

--
-- TOC entry 1909 (class 2604 OID 16390)
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY device ALTER COLUMN id SET DEFAULT nextval('device_id_seq'::regclass);


--
-- TOC entry 1912 (class 2604 OID 16429)
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY division ALTER COLUMN id SET DEFAULT nextval('division_id_seq'::regclass);


--
-- TOC entry 1910 (class 2604 OID 16404)
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY person ALTER COLUMN id SET DEFAULT nextval('person_id_seq'::regclass);


--
-- TOC entry 1914 (class 2604 OID 16532)
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY report ALTER COLUMN id SET DEFAULT nextval('report_id_seq'::regclass);


--
-- TOC entry 1913 (class 2604 OID 16514)
-- Name: version; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY rules ALTER COLUMN version SET DEFAULT nextval('rules_version_seq'::regclass);


--
-- TOC entry 1911 (class 2604 OID 16415)
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY svt ALTER COLUMN id SET DEFAULT nextval('svt_id_seq'::regclass);


--
-- TOC entry 2049 (class 0 OID 16385)
-- Dependencies: 171
-- Data for Name: device; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY device (id, "VID", "PID", serial, manufacturer, product, title, comment, person_id, reg_num) FROM stdin;
2	090c	1000	4223232bc	JetFlash	uf-50	Silicon Motion, Inc. - Taiwan (formerly Feiya Technology Corp.) Flash Drive		5	654 ЕН
3	1009	213F	725553343EE	Transcend	JF-32	JetFlash		6	766 ПР
1	1009	5633	1299FF635	Trancscend	UM-16	Trancend Flash		2	Р-915
4	1765	B371	34523423FF87	Silicon Motion	UF-12-4	Silicon Power Flash 4Gb		9	5634
5	058f	6387	AC46EC14	Generic	Mass Storage	Alcor Micro Corp. Flash Drive	Царицыно	2	444
\.


--
-- TOC entry 2081 (class 0 OID 0)
-- Dependencies: 172
-- Name: device_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('device_id_seq', 5, true);


--
-- TOC entry 2059 (class 0 OID 16494)
-- Dependencies: 181
-- Data for Name: device_to_svt; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY device_to_svt (device_id, svt_id) FROM stdin;
1	8
1	3
1	2
1	1
1	7
1	6
1	5
1	4
4	8
4	3
4	2
4	1
4	7
4	6
4	5
4	4
5	8
2	7
2	8
2	1
2	2
2	3
2	4
2	5
2	6
3	7
3	6
3	4
\.


--
-- TOC entry 2055 (class 0 OID 16424)
-- Dependencies: 177
-- Data for Name: division; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY division (id, title, comment) FROM stdin;
1	1-й отдел	
5	2-й отдел	+ гараж
6	3-й отдел	
8	4-й отдел	+ дежурная служба
9	5-е отделение	
10	Отдел кадров	
11	Секретариат	
\.


--
-- TOC entry 2082 (class 0 OID 0)
-- Dependencies: 178
-- Name: division_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('division_id_seq', 11, true);


--
-- TOC entry 2051 (class 0 OID 16399)
-- Dependencies: 173
-- Data for Name: person; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY person (id, fam, name, otch, comment) FROM stdin;
1	Иванов	Иван	Иванович	каб. 78
2	Александров	Александр	Александрович	Начальник ОБР
3	Петров	Петр	Петрович	оперативный дежурный
4	Сидоров	Кирилл	Сергеевич	завхоз
5	Ишмуратов	Фанзиль	Маратович	
6	Карнаухов	Андрей	Валентинович	
7	Еланкин	Александр	Владимирович	
8	Никоненко	Владислав	Петрович	
9	Хабибулин	Рустам	Фяридович	
\.


--
-- TOC entry 2083 (class 0 OID 0)
-- Dependencies: 174
-- Name: person_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('person_id_seq', 9, true);


--
-- TOC entry 2057 (class 0 OID 16433)
-- Dependencies: 179
-- Data for Name: person_to_division; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY person_to_division (person_id, division_id) FROM stdin;
1	1
2	8
2	6
3	8
4	9
6	10
7	10
8	11
9	1
\.


--
-- TOC entry 2063 (class 0 OID 16529)
-- Dependencies: 185
-- Data for Name: report; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY report (id, date, host, "PID", "VID", serial, comment, manufacturer, product) FROM stdin;
1	2017-02-26 09:12:59.313104	\N	\N	\N	\N	\N	\N	\N
2	2017-02-26 16:12:40.131695	192.168.1.9	1000	090c	1109119100001129	\N	UFD 2.0	Silicon-Power16G
3	2017-02-26 16:22:19.302574	192.168.1.9	1000	090c	1109119100001129	\N	UFD 2.0	Silicon-Power16G
4	2017-02-26 16:26:07.449953	192.168.1.9	0000	0000	FFFFFFFF	\N	\\	Mass Storage
5	2017-02-26 16:27:58.006023	192.168.1.9	1000	090c	1109119100001129	\N	UFD 2.0	Silicon-Power16G
6	2017-02-26 16:57:56.309984	192.168.1.9	1000	090c	1109119100001129	\N	UFD 2.0	Silicon-Power16G
7	2017-02-26 16:58:11.866982	192.168.1.9	5106	174c	11P9F2BOS	\N	StoreJet Transcend	StoreJet Transcend
8	2017-02-26 16:59:05.325735	192.168.1.9	6366	058f	058F63666471	\N	Generic	Mass Storage Device
\.


--
-- TOC entry 2084 (class 0 OID 0)
-- Dependencies: 184
-- Name: report_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('report_id_seq', 8, true);


--
-- TOC entry 2060 (class 0 OID 16509)
-- Dependencies: 182
-- Data for Name: rules; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY rules (version, date, svt_id) FROM stdin;
23	2017-02-25 22:43:28.342513	5
24	2017-02-25 22:43:28.342513	6
25	2017-02-25 22:43:28.342513	7
26	2017-02-25 22:44:10.773483	4
27	2017-02-25 22:44:10.773483	5
28	2017-02-25 22:44:10.773483	6
29	2017-02-25 22:44:10.773483	7
30	2017-02-25 22:45:18.254716	8
31	2017-02-25 22:45:18.254716	7
32	2017-02-25 22:45:18.254716	6
33	2017-02-25 22:45:18.254716	5
34	2017-02-25 22:45:18.254716	4
35	2017-02-25 22:45:18.254716	1
18	2017-02-25 22:11:11.525005	5
36	2017-02-25 22:47:34.076467	4
37	2017-02-25 22:47:34.076467	6
38	2017-02-25 22:47:34.076467	7
39	2017-02-25 22:47:34.076467	8
40	2017-02-25 23:10:18.456059	7
41	2017-02-25 23:10:18.456059	8
42	2017-02-25 23:10:18.456059	1
43	2017-02-25 23:10:18.456059	2
44	2017-02-25 23:10:18.456059	3
45	2017-02-25 23:10:18.456059	4
46	2017-02-25 23:10:18.456059	5
47	2017-02-25 23:10:18.456059	6
48	2017-02-25 23:14:32.966686	1
49	2017-02-25 23:14:32.966686	7
50	2017-02-25 23:14:32.966686	6
51	2017-02-25 23:14:32.966686	5
52	2017-02-25 23:14:32.966686	4
53	2017-02-25 23:14:32.966686	3
54	2017-02-25 23:14:32.966686	2
55	2017-02-25 23:15:12.620163	7
56	2017-02-25 23:15:12.620163	6
57	2017-02-25 23:15:12.620163	4
58	2017-02-25 23:19:48.685886	8
59	2017-02-25 23:27:14.678794	8
60	2017-02-25 23:28:08.32058	8
61	2017-02-25 23:28:08.32058	3
62	2017-02-25 23:28:08.32058	2
63	2017-02-25 23:28:08.32058	1
64	2017-02-25 23:28:08.32058	7
65	2017-02-25 23:28:08.32058	6
66	2017-02-25 23:28:08.32058	5
67	2017-02-25 23:28:08.32058	4
68	2017-02-25 23:28:45.503939	8
69	2017-02-25 23:28:45.503939	3
70	2017-02-25 23:28:45.503939	2
71	2017-02-25 23:28:45.503939	1
72	2017-02-25 23:28:45.503939	7
73	2017-02-25 23:28:45.503939	6
74	2017-02-25 23:28:45.503939	5
75	2017-02-25 23:28:45.503939	4
76	2017-02-25 23:51:48.445393	8
\.


--
-- TOC entry 2085 (class 0 OID 0)
-- Dependencies: 183
-- Name: rules_version_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('rules_version_seq', 76, true);


--
-- TOC entry 2053 (class 0 OID 16410)
-- Dependencies: 175
-- Data for Name: svt; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY svt (id, title, address, "user", password, comment, person_id, deploy_time, rule_version) FROM stdin;
3	PC-05	192.168.1.105	root	root	каб 306	1	\N	\N
8	astra1	192.168.1.9	root	root		\N	2017-02-26 16:27:46.402315	76
2	PC-03	192.168.1.103	root	root		2	\N	\N
4	MacPro	192.6.55.232	root	root	по блату	2	\N	\N
6	kadr-01	192.168.10.13	root	root		7	\N	\N
7	mac book	192.168.11.46	root	root		9	\N	\N
1	PC-01	192.168.1.101	root	root	каб. 105	2	\N	\N
5	PC-12	192.168.4.112	root	root	сервер ЭП	7	\N	\N
\.


--
-- TOC entry 2086 (class 0 OID 0)
-- Dependencies: 176
-- Name: svt_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('svt_id_seq', 8, true);


--
-- TOC entry 2058 (class 0 OID 16452)
-- Dependencies: 180
-- Data for Name: svt_to_division; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY svt_to_division (svt_id, division_id) FROM stdin;
3	5
3	1
4	8
2	6
6	10
5	5
1	5
\.


--
-- TOC entry 1916 (class 2606 OID 16395)
-- Name: device_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY device
    ADD CONSTRAINT device_pkey PRIMARY KEY (id);


--
-- TOC entry 1928 (class 2606 OID 16498)
-- Name: device_to_svt_pk; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY device_to_svt
    ADD CONSTRAINT device_to_svt_pk PRIMARY KEY (device_id, svt_id);


--
-- TOC entry 1922 (class 2606 OID 16444)
-- Name: division_pk; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY division
    ADD CONSTRAINT division_pk PRIMARY KEY (id);


--
-- TOC entry 1918 (class 2606 OID 16409)
-- Name: person_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY person
    ADD CONSTRAINT person_pkey PRIMARY KEY (id);


--
-- TOC entry 1924 (class 2606 OID 16488)
-- Name: person_to_division_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY person_to_division
    ADD CONSTRAINT person_to_division_pkey PRIMARY KEY (person_id, division_id);


--
-- TOC entry 1932 (class 2606 OID 16537)
-- Name: report_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY report
    ADD CONSTRAINT report_pkey PRIMARY KEY (id);


--
-- TOC entry 1930 (class 2606 OID 16519)
-- Name: rules_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY rules
    ADD CONSTRAINT rules_pkey PRIMARY KEY (version);


--
-- TOC entry 1920 (class 2606 OID 16420)
-- Name: svt_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY svt
    ADD CONSTRAINT svt_pkey PRIMARY KEY (id);


--
-- TOC entry 1926 (class 2606 OID 16456)
-- Name: svt_to_division_pk; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY svt_to_division
    ADD CONSTRAINT svt_to_division_pk PRIMARY KEY (svt_id, division_id);


--
-- TOC entry 2048 (class 2618 OID 16538)
-- Name: report_inserted; Type: RULE; Schema: public; Owner: postgres
--

CREATE RULE report_inserted AS
    ON INSERT TO report DO  SELECT pg_notify('report'::text, 'inserted'::text) AS pg_notify;


--
-- TOC entry 1933 (class 2606 OID 16482)
-- Name: device_person_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY device
    ADD CONSTRAINT device_person_fk FOREIGN KEY (person_id) REFERENCES person(id) ON UPDATE CASCADE ON DELETE SET NULL;


--
-- TOC entry 1939 (class 2606 OID 16499)
-- Name: device_to_svt_device_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY device_to_svt
    ADD CONSTRAINT device_to_svt_device_fk FOREIGN KEY (device_id) REFERENCES device(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 1940 (class 2606 OID 16504)
-- Name: device_to_svt_svt_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY device_to_svt
    ADD CONSTRAINT device_to_svt_svt_fk FOREIGN KEY (svt_id) REFERENCES svt(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 1936 (class 2606 OID 16445)
-- Name: person_to_division_division_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY person_to_division
    ADD CONSTRAINT person_to_division_division_fk FOREIGN KEY (division_id) REFERENCES division(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 1935 (class 2606 OID 16438)
-- Name: person_to_division_person_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY person_to_division
    ADD CONSTRAINT person_to_division_person_fk FOREIGN KEY (person_id) REFERENCES person(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 1934 (class 2606 OID 16489)
-- Name: svt_person_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY svt
    ADD CONSTRAINT svt_person_fk FOREIGN KEY (person_id) REFERENCES person(id) ON UPDATE CASCADE ON DELETE SET NULL;


--
-- TOC entry 1938 (class 2606 OID 16462)
-- Name: svt_to_division_division_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY svt_to_division
    ADD CONSTRAINT svt_to_division_division_fk FOREIGN KEY (division_id) REFERENCES division(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 1937 (class 2606 OID 16457)
-- Name: svt_to_division_svt_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY svt_to_division
    ADD CONSTRAINT svt_to_division_svt_fk FOREIGN KEY (svt_id) REFERENCES svt(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2070 (class 0 OID 0)
-- Dependencies: 6
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- TOC entry 2075 (class 0 OID 0)
-- Dependencies: 185
-- Name: report; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE report FROM PUBLIC;
REVOKE ALL ON TABLE report FROM postgres;
GRANT ALL ON TABLE report TO postgres;
GRANT INSERT ON TABLE report TO agent;


--
-- TOC entry 2077 (class 0 OID 0)
-- Dependencies: 184
-- Name: report_id_seq; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON SEQUENCE report_id_seq FROM PUBLIC;
REVOKE ALL ON SEQUENCE report_id_seq FROM postgres;
GRANT ALL ON SEQUENCE report_id_seq TO postgres;
GRANT USAGE ON SEQUENCE report_id_seq TO agent;


-- Completed on 2017-02-26 17:05:01 MSK

--
-- PostgreSQL database dump complete
--

