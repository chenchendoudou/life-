深入理解计算机系统代码

闲来无事瞎看看
## 🖥️ 公司电脑（分支 workCD）

```
# 1.切到你的工作分支开发
git checkout workCD
# 写代码，本地提交
git add .
git commit -m "xxx功能"

# 2.拉取远程最新代码
git fetch origin

# 3！！重点：用rebase同步main，不要merge main
git rebase origin/main

# 4.切到main，合并上去（此时是快进合并，**不会生成新merge提交**）
git checkout main
git merge workCD
git push origin main
```

## 🏠 家里电脑（分支 yylxl）

```
# 1.拿到远端最新main
git checkout main
git pull origin main

# 2.切回自己的开发分支
git checkout yylxl
git fetch origin

# 3！！重点，禁止 git merge main
git rebase origin/main

# 在家写代码、提交
git add .
git commit -m "在家改动"

# 4.合并进主分支推送
git checkout main
git merge yylxl
git push origin main
```

## 核心铁律（背下来）

1. **单向流向：功能分支 → main，永远不可逆**
2. 更新本地功能分支（`workCD` / `yylxl`）：**只用 rebase，不要 merge main**
3. `main` 只进不出：永远不要把 main 合并回你的开发分支