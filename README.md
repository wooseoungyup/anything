# MARK DOWN 정리
## 1. 마크다운 이란   
마크다운은 텍스트기반의 마크업언어입니다. 쉽게 쓰고 읽을 수 있으며,          
간단히 기록하고
가독성을 높일 수 있습니다.
***
## 2. 마크다운 사용법(문법)
### 1. 헤더

헤더의 크기를 # 을 통해서 조절 할 수 있습니다. 
샵(#) 의 개수에 따라 글자의 크기가 정해지며, 샵(#)이 많을 수록 글자가 작아집니다.
아래는 예시 입니다.

```
# H1
##  H2
###  H3
####  H4
#####  H5
######  H6

```

# H1
##  H2
###  H3
####  H4
#####  H5
######  H6

### 2. 블럭인용문자

">" 를 이용함으로서 아래 예시와 같은 표현이 가능합니다.

```
> 예시입니다.
>  > 예시입니다.
>  >  > 예시입니다.
```

> 예시입니다.
>  > 예시입니다.
>  >  > 예시입니다.

### 3. 목록

단순히 숫자를 이용하여 목록을 나타 낼 수 있습니다.

```
1. 일번
2. 이번
3. 삼번
```
1. 일번
2. 이번
3. 삼번

또한 [* , +, -] 를 이용하여 점, 원 ,사각형과 같은 것을 표현 할 수 있습니다.
```
* 빨강
  * 녹색
    * 파랑

+ 빨강
  + 녹색
    + 파랑

- 빨강
  - 녹색
    - 파랑

```
* 빨강
  * 녹색
    * 파랑

+ 빨강
  + 녹색
    + 파랑

- 빨강
  - 녹색
    - 파랑


### 4.코드블럭
기호 \``` ``` 를 이용하여 코드블럭을 이용할 수 있습니다.

<pre>
<code>
```
이렇게 사용 할 수 있습니다.
```
</code>
</pre>

```
이렇게 사용 할 수 있습니다.
```

### 5.링크

1. 참조링크
```
[link keyword][id]

[id]: URL "Optional Title here"

// code
Link: [Google][googlelink]

[googlelink]: https://google.com "구글가요"
```
Link: [Google][googlelink]

[googlelink]: https://google.com "구글가요"       


2. 외부링크
```
사용문법: [Title](link)
적용예: [Google](https://google.com, "google link")
```
사용문법: [Title](link)
적용예: [Google](https://google.com, "google link")

3. 자동연결
```
일반적인 URL 혹은 이메일주소인 경우 적절한 형식으로 링크를 형성한다.

* 외부링크: <http://example.com/>
* 이메일링크: <address@example.com>
```
일반적인 URL 혹은 이메일주소인 경우 적절한 형식으로 링크를 형성한다.

* 외부링크: <http://example.com/>
* 이메일링크: <address@example.com>


### 6. 이미지
아래와 같이 사용가능합니다.

```
![Alt text](/path/to/img.jpg)
![Alt text](/path/to/img.jpg "Optional title")
```

***
#### 참고 자료 출처
아래의 링크의 자료를 참고하여 작성하였습니다.
<https://gist.github.com/ihoneymon/652be052a0727ad59601>

